#include "VulkanRenderer.h"
#include <utility>

#include "VulkanImageAssetData.h"
#include "VulkanShaderAssetData.h"

#include "VulkanUniversalData.h"
#include "VulkanPipelineAssemblyOrigin.h"
#include "VulkanUtilities.h"
#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    Renderer::Renderer(
        std::shared_ptr<vk::Device> device,
        vk::Sampler sampler,
        LineShaders lineShaders,
        vk::CommandPool commandPool,
        vk::Queue graphicsQueue,
        vk::Queue presentQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        Arca::Reliquary& reliquary)
        :
        device(device),
        lineShaders(std::move(lineShaders)),
        sampler(sampler),
        memoryProperties(memoryProperties),
        texturedVertexBuffer(texturedVertexStride * sizeof(TexturedVertex), *device, memoryProperties, vk::BufferUsageFlagBits::eVertexBuffer),
        texturedIndexBuffer(texturedIndexStride * sizeof(std::int16_t), *device, memoryProperties, vk::BufferUsageFlagBits::eIndexBuffer),
        lineVertexBuffer(lineVertexStride * sizeof(LineVertex), *device, memoryProperties, vk::BufferUsageFlagBits::eVertexBuffer),
        graphicsQueue(graphicsQueue),
        presentQueue(presentQueue),
        commandPool(commandPool),
        reliquary(&reliquary)
    {
        imageAvailableSemaphores = CreateSemaphores(*device, maxFramesInFlight);
        renderFinishedSemaphores = CreateSemaphores(*device, maxFramesInFlight);

        inFlightFences = CreateFences(*device, maxFramesInFlight);
    }

    Renderer::~Renderer()
    {
        device->waitIdle();

        commandBuffers.clear();
        inFlightFences.clear();
    }

    void Renderer::Initialize(
        vk::SwapchainKHR swapchain,
        std::vector<vk::Image> images,
        std::vector<vk::ImageView> imageViews,
        vk::Format imageFormat,
        vk::Extent2D extent)
    {
        this->swapchain = swapchain;
        this->images = images;
        this->imageViews = imageViews;
        this->extent = extent;

        renderPass = CreateRenderPass(*device, imageFormat);

        framebuffers = CreateFramebuffers(
            *device, imageViews, renderPass.get(), extent);

        materialAssembly = PipelineAssemblyOrigin(this->images.size())
            .UniformBuffer(0)
            .CombinedImageSampler(1)
            .Assembly(*device);
        lineAssembly = PipelineAssemblyOrigin(this->images.size())
            .UniformBuffer(0)
            .Assembly(*device);

        commandBuffers = CreateCommandBuffers(*device, commandPool, images.size());
        imagesInFlight.resize(images.size(), nullptr);

        auto allMaterials = reliquary->Batch<Asset::MaterialAsset>();
        std::vector<const Asset::MaterialAsset*> materialsVector;
        for (auto& material : allMaterials)
            materialsVector.push_back(&material);
        materialPipelines = CreateAllMaterialPipelines(materialsVector, extent);

        linePipeline = CreateLinePipeline(extent);

        for(auto& pipeline : materialPipelines)
        {
            for (std::uint32_t i = 0; i < pipeline.uniformBuffers.size(); ++i)
            {
                const auto writeDescriptor =
                    pipeline.uniformBufferDescriptors[i].Write(materialAssembly->descriptorSets[i]);
                device->updateDescriptorSets(1, &writeDescriptor, 0, nullptr);
            }
        }

        {
            for (std::uint32_t i = 0; i < linePipeline.uniformBuffers.size(); ++i)
            {
                auto writeDescriptor =
                    linePipeline.uniformBufferDescriptors[i].Write(lineAssembly->descriptorSets[i]);
                device->updateDescriptorSets(1, &writeDescriptor, 0, nullptr);
            }
        }
    }

    void Renderer::StageRender(const ImageRender& imageRender)
    {
        const auto halfWidth = imageRender.size.width / 2;
        const auto halfHeight = imageRender.size.height / 2;
        const auto position = imageRender.position;
        const auto z = AtmosToVulkanZ(imageRender.position.z);
        const auto imageAsset = imageRender.asset;
        const auto material = imageRender.material;
        const auto color = AtmosToVulkanColor(imageRender.color);

        const std::array<TexturedVertex, 4> vertices
        {
            TexturedVertex
            {
                { position.x - halfWidth, position.y - halfHeight, z },
                { 0.0f, 0.0f },
                color
            },
            TexturedVertex
            {
                { position.x + halfWidth, position.y - halfHeight, z },
                { 1.0f, 0.0f },
                color
            },
            TexturedVertex
            {
                { position.x - halfWidth, position.y + halfHeight, z },
                { 0.0f, 1.0f },
                color
            },
            TexturedVertex
            {
                { position.x + halfWidth, position.y + halfHeight, z },
                { 1.0f, 1.0f },
                color
            }
        };
        auto& layer = LayerFor(z);
        auto pipeline = PipelineFor(*material);
        auto& context = layer.ContextFor(*pipeline);
        auto quadGroup = context.QuadGroupFor(imageAsset);
        if (!quadGroup)
            quadGroup = &context.quadGroups.emplace(
                imageAsset,
                Layer::QuadContext::QuadGroup(imageAsset->FileDataAs<ImageAssetDataImplementation>()->descriptor))
                .first->second;
        quadGroup->quads.emplace_back(vertices);
    }

    void Renderer::StageRender(const LineRender& lineRender)
    {
        const auto z = AtmosToVulkanZ(lineRender.z);

        std::vector<LineVertex> points;
        for (auto& point : lineRender.points)
        {
            const auto color = AtmosToVulkanColor(lineRender.color);
            points.push_back(LineVertex{
                { point.x, point.y, z },
                color });
        }

        auto& layer = LayerFor(z);
        auto& context = layer.ContextFor(lineRender.width);
        context.lines.emplace_back(points, lineRender.color, z, lineRender.width);
    }

    void Renderer::DrawFrame(const ScreenSize& cameraSize)
    {
        device->waitForFences(inFlightFences[previousFrame].get(), VK_TRUE, UINT64_MAX);

        if (layers.empty())
            return;

        auto imageIndex = device->acquireNextImageKHR(
            swapchain,
            UINT64_MAX,
            imageAvailableSemaphores[currentFrame].get(),
            nullptr);
        if (imageIndex.result == vk::Result::eErrorOutOfDateKHR)
        {
            onOutOfDate();
            return;
        }
        else if (IsError(imageIndex.result) && imageIndex.result != vk::Result::eSuboptimalKHR)
            throw GraphicsError("Could not acquire next image in swapchain.");

        if (imagesInFlight[imageIndex.value])
            device->waitForFences(1, &imagesInFlight[imageIndex.value], VK_TRUE, UINT64_MAX);
        imagesInFlight[imageIndex.value] = inFlightFences[currentFrame].get();

        auto commandBuffer = commandBuffers[imageIndex.value].get();

        const vk::CommandBufferBeginInfo beginInfo({}, nullptr);
        commandBuffer.begin(beginInfo);

        DrawAll(
            commandBuffer,
            imageIndex.value,
            glm::vec2 { cameraSize.width, cameraSize.height });

        commandBuffer.end();

        vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame].get() };
        vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame].get() };
        vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
        vk::SubmitInfo submitInfo(1, waitSemaphores, waitStages, 1, &commandBuffer, 1, signalSemaphores);

        device->resetFences(1, &inFlightFences[currentFrame].get());

        graphicsQueue.submit(1, &submitInfo, inFlightFences[currentFrame].get());
        
        const vk::SwapchainKHR swapchains[] = { swapchain };
        vk::PresentInfoKHR presentInfo(1, signalSemaphores, 1, swapchains, &imageIndex.value, nullptr);
        const auto presentResult = presentQueue.presentKHR(&presentInfo);
        if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR)
            onOutOfDate();
        else if (IsError(presentResult))
            throw GraphicsError("Could not present swap chain.");

        previousFrame = currentFrame;
        currentFrame = (currentFrame + 1) % maxFramesInFlight;
    }

    Renderer::Line::Line(const std::vector<LineVertex>& points, const Color& color, float z, LineWidth width)
        :
        vertices(points), color(color), z(z), width(width)
    {}

    auto Renderer::Layer::QuadContext::QuadGroupFor(const Asset::ImageAsset* imageAsset) -> QuadGroup*
    {
        auto found = quadGroups.find(imageAsset);
        if (found == quadGroups.end())
            return nullptr;

        return &found->second;
    }

    Renderer::Layer::Layer(Position3D::Value z) : z(z)
    {}

    auto Renderer::Layer::ContextFor(Pipeline& pipeline) -> QuadContext&
    {
        for (auto& context : quadContexts)
            if (context.pipeline == &pipeline)
                return context;

        quadContexts.emplace_back(pipeline);
        return quadContexts.back();
    }

    auto Renderer::Layer::ContextFor(LineWidth width) -> LineContext&
    {
        auto found = lineContexts.find(width);
        if (found == lineContexts.end())
            return lineContexts.emplace(width, LineContext{}).first->second;

        return found->second;
    }

    auto Renderer::LayerFor(Position3D::Value z) -> Layer&
    {
        for (auto& layer : layers)
            if (layer.z == z)
                return layer;

        layers.emplace_back(z);
        return layers.back();
    }

    Position3D::Value Renderer::AtmosToVulkanZ(Position3D::Value input)
    {
        const auto atmosDifferential = std::numeric_limits<float>::max();
        const auto vulkanDifferential = 1.0f;
        const auto atmosMin = std::numeric_limits<float>::min() / 2;
        const auto vulkanMin = 0.0f;
        return (input - atmosMin) * vulkanDifferential / atmosDifferential + vulkanMin;
    }

    void Renderer::DrawAll(
        vk::CommandBuffer commandBuffer,
        std::uint32_t currentImage,
        glm::vec2 cameraSize)
    {
        if (layers.empty())
            return;

        std::sort(
            layers.begin(),
            layers.end(),
            [](const Layer& left, const Layer& right)
            {
                return left.z < right.z;
            });

        const vk::ClearValue clearColor(vk::ClearColorValue(std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f }));
        const vk::RenderPassBeginInfo renderPassBeginInfo(
            renderPass.get(),
            framebuffers[currentImage].get(),
            vk::Rect2D({ 0, 0 }, extent),
            1,
            &clearColor);

        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        for (auto& layer : layers)
        {
            for (auto& context : layer.quadContexts)
                if (!context.quadGroups.empty())
                    for (auto& group : context.quadGroups)
                        DrawAllQuads(
                            group.second.quads,
                            *context.pipeline,
                            group.second.descriptor,
                            commandBuffer,
                            currentImage,
                            cameraSize);

            for(auto& context : layer.lineContexts)
                if (!context.second.lines.empty())
                    DrawAllLines(context.second.lines, context.first, commandBuffer, currentImage, cameraSize);
        }
        layers.clear();

        commandBuffer.endRenderPass();
    }

    void Renderer::DrawAllQuads(
        const std::vector<Quad>& quads,
        Pipeline& pipeline,
        CombinedImageSamplerDescriptor descriptor,
        vk::CommandBuffer commandBuffer,
        std::uint32_t currentImage,
        glm::vec2 cameraSize)
    {
        auto currentDescriptorSet = materialAssembly->descriptorSets[currentImage];

        const auto writeDescriptor = descriptor.Write(currentDescriptorSet);
        device->updateDescriptorSets(1, &writeDescriptor, 0, nullptr);

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.value.get());

        vk::Buffer vertexBuffers[] = { texturedVertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        commandBuffer.bindIndexBuffer(texturedIndexBuffer.destination.value.get(), 0, vk::IndexType::eUint16);

        pipeline.uniformBuffers[currentImage].PushBytes(UniversalData::Ortho(cameraSize), 0);

        commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            pipeline.layout.get(),
            0,
            1,
            &currentDescriptorSet,
            0,
            nullptr);

        std::uint32_t vertexCount = 0;
        for (auto& quad : quads)
        {
            const auto quadVertexCount = quad.vertices.size();

            if (quadVertexCount + vertexCount > lineVertexStride)
            {
                commandBuffer.drawIndexed(quadIndices.size(), 1, 0, 0, 0);
                vertexCount = 0;
            }

            texturedVertexBuffer.source.PushBytes(
                quad.vertices.data(),
                vertexCount * std::uint64_t(sizeof(TexturedVertex)),
                quadVertexCount * sizeof(TexturedVertex));

            texturedVertexBuffer.CopyFromSourceToDestination(commandPool, graphicsQueue);

            texturedIndexBuffer.source.PushBytes(
                quadIndices.data(),
                vertexCount * sizeof(std::int16_t),
                quadIndices.size() * sizeof(std::int16_t));

            texturedIndexBuffer.CopyFromSourceToDestination(commandPool, graphicsQueue);

            vertexCount += quadVertexCount;
        }

        if (vertexCount > 0)
            commandBuffer.drawIndexed(quadIndices.size(), 1, 0, 0, 0);
    }

    void Renderer::DrawAllLines(
        const std::vector<Line>& lines,
        LineWidth width,
        vk::CommandBuffer commandBuffer,
        std::uint32_t currentImage,
        glm::vec2 cameraSize)
    {
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, linePipeline.value.get());

        vk::Buffer vertexBuffers[] = { lineVertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        linePipeline.uniformBuffers[currentImage].PushBytes(UniversalData::Ortho(cameraSize), 0);

        commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            linePipeline.layout.get(),
            0,
            1,
            &lineAssembly->descriptorSets[currentImage],
            0,
            nullptr);

        commandBuffer.setLineWidth(width);

        std::uint32_t vertexCount = 0;
        for (auto& line : lines)
        {
            const auto lineVertexCount = line.vertices.size();

            if (lineVertexCount + vertexCount > lineVertexStride)
            {
                commandBuffer.draw(vertexCount, 1, 0, 0);
                vertexCount = 0;
            }

            lineVertexBuffer.PushSourceBytes(
                line.vertices.data(),
                vertexCount * uint64_t(sizeof(LineVertex)),
                lineVertexCount * sizeof(LineVertex));

            lineVertexBuffer.CopyFromSourceToDestination(commandPool, graphicsQueue);

            vertexCount += lineVertexCount;
        }

        if (vertexCount > 0)
            commandBuffer.draw(vertexCount, 1, 0, 0);
    }

    void Renderer::ClearImage(vk::Image image, std::array<float, 4> color, vk::CommandBuffer commandBuffer)
    {
        const vk::ImageSubresourceRange subresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };

        const vk::ClearColorValue clearColor(color);
        commandBuffer.clearColorImage(image, vk::ImageLayout::eUndefined, clearColor, subresourceRange);
    }

    auto Renderer::CreateAllMaterialPipelines(
        const std::vector<const Asset::MaterialAsset*>& materials,
        vk::Extent2D extent)
        ->
        std::vector<Pipeline>
    {
        std::vector<Pipeline> returnValue;

        for (auto& material : materials)
        {
            auto layout = CreatePipelineLayout(*device, materialAssembly->descriptorSetLayout.get());

            const auto vertexShader = material->VertexShader().Get();
            const auto fragmentShader = material->FragmentShader().Get();
            std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
            if (vertexShader)
                shaderStages.push_back(ShaderStageCreateInfo(*vertexShader, vk::ShaderStageFlagBits::eVertex));
            if (fragmentShader)
                shaderStages.push_back(ShaderStageCreateInfo(*fragmentShader, vk::ShaderStageFlagBits::eFragment));
            auto value = CreatePipeline(
                *device,
                shaderStages,
                layout.get(),
                renderPass.get(),
                TexturedVertexInput(),
                extent,
                vk::PrimitiveTopology::eTriangleList);

            auto uniformBuffers = CreateUniformBuffers(*device, memoryProperties, imageViews.size());

            returnValue.emplace_back(
                std::move(value),
                std::move(layout),
                std::move(uniformBuffers),
                extent);
        }

        return returnValue;
    }

    auto Renderer::CreateLinePipeline(
        vk::Extent2D extent) -> Pipeline
    {
        auto layout = CreatePipelineLayout(*device, lineAssembly->descriptorSetLayout.get());

        const auto vertexShader = lineShaders.vertex.Get();
        const auto fragmentShader = lineShaders.fragment.Get();
        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        if (vertexShader)
            shaderStages.push_back(ShaderStageCreateInfo(*vertexShader, vk::ShaderStageFlagBits::eVertex));
        if (fragmentShader)
            shaderStages.push_back(ShaderStageCreateInfo(*fragmentShader, vk::ShaderStageFlagBits::eFragment));
        auto value = CreatePipeline(
            *device,
            shaderStages,
            layout.get(),
            renderPass.get(),
            LineVertexInput(),
            extent,
            vk::PrimitiveTopology::eLineList);

        auto uniformBuffers = CreateUniformBuffers(*device, memoryProperties, imageViews.size());

        return Pipeline(
            std::move(value),
            std::move(layout),
            std::move(uniformBuffers),
            extent);
    }

    auto Renderer::PipelineFor(const Asset::MaterialAsset& material) -> Pipeline*
    {
        for (auto& pipeline : materialPipelines)
            if (pipeline.material == &material)
                return &pipeline;

        return nullptr;
    }

    std::vector<Pipeline*> Renderer::AllPipelines()
    {
        std::vector<Pipeline*> allPipelines;
        for (auto& pipeline : materialPipelines)
            allPipelines.push_back(&pipeline);
        allPipelines.push_back(&linePipeline);
        return allPipelines;
    }

    VertexInput Renderer::TexturedVertexInput()
    {
        return VertexInput(
            sizeof(TexturedVertex),
            {
                vk::VertexInputAttributeDescription(
                    0, 0, vk::Format::eR32G32B32Sfloat, offsetof(TexturedVertex, position)),
                vk::VertexInputAttributeDescription(
                    1, 0, vk::Format::eR32G32Sfloat, offsetof(TexturedVertex, texture)),
                vk::VertexInputAttributeDescription(
                    2, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(TexturedVertex, color))
            });
    }

    VertexInput Renderer::LineVertexInput()
    {
        return VertexInput(
            sizeof(LineVertex),
            {
                vk::VertexInputAttributeDescription(
                    0, 0, vk::Format::eR32G32B32Sfloat, offsetof(LineVertex, position)),
                vk::VertexInputAttributeDescription(
                    1, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(LineVertex, color))
            });
    }

    vk::UniqueRenderPass Renderer::CreateRenderPass(
        vk::Device device, vk::Format swapchainImageFormat)
    {
        vk::AttachmentDescription colorAttachment(
            {},
            swapchainImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR);

        vk::AttachmentReference colorAttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);

        vk::SubpassDescription subpass(
            {},
            vk::PipelineBindPoint::eGraphics,
            {},
            {},
            1,
            &colorAttachmentReference);

        vk::SubpassDependency dependency(
            VK_SUBPASS_EXTERNAL,
            0,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            {},
            vk::AccessFlagBits::eColorAttachmentWrite);

        const vk::RenderPassCreateInfo createInfo({}, 1, &colorAttachment, 1, &subpass, 1, &dependency);

        return device.createRenderPassUnique(createInfo);
    }

    std::vector<vk::UniqueFramebuffer> Renderer::CreateFramebuffers(
        vk::Device device, const std::vector<vk::ImageView>& imageViews, vk::RenderPass renderPass, vk::Extent2D extent)
    {
        std::vector<vk::UniqueFramebuffer> returnValue(imageViews.size());
        for (size_t i = 0; i < imageViews.size(); ++i)
        {
            vk::ImageView attachments[] =
            {
                imageViews[i]
            };

            vk::FramebufferCreateInfo createInfo(
                {},
                renderPass,
                1,
                attachments,
                extent.width,
                extent.height,
                1);

            returnValue[i] = device.createFramebufferUnique(createInfo);
        }
        return returnValue;
    }

    vk::UniquePipelineLayout Renderer::CreatePipelineLayout(
        vk::Device device, vk::DescriptorSetLayout descriptorSetLayout)
    {
        const vk::PipelineLayoutCreateInfo createInfo({}, 1, &descriptorSetLayout);
        return device.createPipelineLayoutUnique(createInfo);
    }

    vk::UniquePipeline Renderer::CreatePipeline(
        vk::Device device,
        const std::vector<vk::PipelineShaderStageCreateInfo>& shaderStages,
        vk::PipelineLayout layout,
        vk::RenderPass renderPass,
        VertexInput vertexInput,
        vk::Extent2D swapchainExtent,
        vk::PrimitiveTopology primitiveTopology,
        std::vector<vk::DynamicState> dynamicStates)
    {
        auto vertexInputCreateInfo = vertexInput.PipelineCreateInfo();

        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo(
            {}, primitiveTopology, VK_FALSE);

        vk::Viewport viewport(
            0.0f,
            static_cast<float>(swapchainExtent.height),
            static_cast<float>(swapchainExtent.width),
            -static_cast<float>(swapchainExtent.height),
            0.0f,
            1.0f);

        vk::Rect2D scissor({ 0, 0 }, swapchainExtent);

        vk::PipelineViewportStateCreateInfo viewportStateCreateInfo({}, 1, &viewport, 1, &scissor);

        vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo(
            {},
            VK_FALSE,
            VK_FALSE,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eBack,
            vk::FrontFace::eCounterClockwise,
            VK_FALSE,
            0.0f,
            0.0f,
            0.0f,
            1.0f);

        vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo(
            {},
            vk::SampleCountFlagBits::e1,
            VK_FALSE,
            1.0f,
            nullptr,
            VK_FALSE,
            VK_FALSE);

        vk::PipelineColorBlendAttachmentState colorBlendAttachment(
            VK_TRUE,
            vk::BlendFactor::eSrcAlpha,
            vk::BlendFactor::eOneMinusSrcAlpha,
            vk::BlendOp::eAdd,
            vk::BlendFactor::eOne,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

        vk::PipelineColorBlendStateCreateInfo colorBlendCreateInfo(
            {},
            VK_FALSE,
            vk::LogicOp::eCopy,
            1,
            &colorBlendAttachment,
            {});

        vk::PipelineDynamicStateCreateInfo dynamicCreateInfo(
            {},
            dynamicStates.size(),
            dynamicStates.data());

        vk::GraphicsPipelineCreateInfo createInfo(
            {},
            shaderStages.size(),
            shaderStages.data(),
            &vertexInputCreateInfo,
            &inputAssemblyCreateInfo,
            nullptr,
            &viewportStateCreateInfo,
            &rasterizerCreateInfo,
            &multisamplingCreateInfo,
            nullptr,
            &colorBlendCreateInfo,
            !dynamicStates.empty() ? &dynamicCreateInfo : nullptr,
            layout,
            renderPass,
            0,
            nullptr,
            0);

        return device.createGraphicsPipelineUnique(nullptr, createInfo);
    }

    vk::PipelineShaderStageCreateInfo Renderer::ShaderStageCreateInfo(
        const Asset::ShaderAsset& shaderAsset, vk::ShaderStageFlagBits shaderType)
    {
        const auto fileData = shaderAsset.FileDataAs<ShaderAssetDataImplementation>();
        return vk::PipelineShaderStageCreateInfo(
            {},
            shaderType,
            fileData->Module(),
            fileData->EntryPoint().c_str());
    }

    glm::vec4 Renderer::AtmosToVulkanColor(const Color& color)
    {
        const auto converter = [](Color::Value input)
        {
            const auto atmosDifferential = std::numeric_limits<Color::Value>::max();
            const auto vulkanDifferential = 1.0f;
            const auto atmosMin = std::uint8_t(0);
            const auto vulkanMin = 0.0f;
            return (input - atmosMin) * vulkanDifferential / atmosDifferential + vulkanMin;
        };

        return
        {
            converter(color.red),
            converter(color.green),
            converter(color.blue),
            converter(color.alpha)
        };
    }

    Renderer::Quad::Quad(const std::array<TexturedVertex, 4>& vertices) : vertices(vertices)
    {}

    std::vector<Buffer> Renderer::CreateUniformBuffers(
        vk::Device device, vk::PhysicalDeviceMemoryProperties memoryProperties, size_t size)
    {
        std::vector<Buffer> returnValue;
        returnValue.reserve(size);
        for (size_t i = 0; i < size; ++i)
            returnValue.emplace_back(
                sizeof(Vulkan::UniversalData),
                vk::BufferUsageFlagBits::eUniformBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                device,
                memoryProperties);
        return returnValue;
    }

    std::vector<vk::UniqueCommandBuffer> Renderer::CreateCommandBuffers(
        vk::Device device, vk::CommandPool commandPool, size_t size)
    {
        std::vector<vk::CommandBuffer> returnValue(size);
        const vk::CommandBufferAllocateInfo allocateInfo(
            commandPool, vk::CommandBufferLevel::ePrimary, size);
        return device.allocateCommandBuffersUnique(allocateInfo);
    }

    std::vector<vk::UniqueSemaphore> Renderer::CreateSemaphores(vk::Device device, size_t count)
    {
        std::vector<vk::UniqueSemaphore> returnValue;

        const vk::SemaphoreCreateInfo createInfo;
        for (size_t i = 0; i < count; ++i)
            returnValue.push_back(device.createSemaphoreUnique(createInfo));

        return returnValue;
    }

    std::vector<vk::UniqueFence> Renderer::CreateFences(vk::Device device, size_t count)
    {
        std::vector<vk::UniqueFence> returnValue;

        const vk::FenceCreateInfo createInfo(vk::FenceCreateFlagBits::eSignaled);
        for (size_t i = 0; i < count; ++i)
            returnValue.push_back(device.createFenceUnique(createInfo));

        return returnValue;
    }
}
