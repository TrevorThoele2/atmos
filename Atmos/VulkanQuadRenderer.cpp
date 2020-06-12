#include "VulkanQuadRenderer.h"

#include "VulkanUniversalData.h"
#include "VulkanImageAssetData.h"
#include "VulkanUtilities.h"

namespace Atmos::Render::Vulkan
{
    QuadRenderer::QuadRenderer(
        std::shared_ptr<vk::Device> device,
        uint32_t graphicsQueueIndex,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties)
        :
        vertexBuffer(vertexStride * sizeof(Vertex), *device, memoryProperties, vk::BufferUsageFlagBits::eVertexBuffer),
        indexBuffer(indexStride * sizeof(std::int16_t), *device, memoryProperties, vk::BufferUsageFlagBits::eIndexBuffer),
        core(
            device,
            memoryProperties,
            VertexInput(
                sizeof(Vertex),
                {
                    vk::VertexInputAttributeDescription(
                        0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)),
                    vk::VertexInputAttributeDescription(
                        1, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, texture)),
                    vk::VertexInputAttributeDescription(
                        2, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, color))
                }),
            {
                DescriptorSetGroup::Definition
                {
                    vk::DescriptorType::eUniformBuffer,
                    0,
                    vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex,
                    1
                },
                DescriptorSetGroup::Definition
                {
                    vk::DescriptorType::eCombinedImageSampler,
                    1,
                    vk::ShaderStageFlagBits::eFragment,
                    1
                }
            },
            vk::PrimitiveTopology::eTriangleList,
            Asset::MaterialType::Image),
        graphicsQueue(graphicsQueue),
        commandBuffers(*device, graphicsQueueIndex),
        device(device)
    {}

    void QuadRenderer::Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent)
    {
        core.Initialize(swapchainImageCount, renderPass, extent);
    }

    void QuadRenderer::StageRender(const ImageRender& imageRender)
    {
        const auto halfWidth = imageRender.size.width / 2;
        const auto halfHeight = imageRender.size.height / 2;
        const auto position = imageRender.position;
        const auto imageAsset = imageRender.asset;
        const auto slice = imageRender.assetSlice;
        const auto materialAsset = imageRender.material;
        const auto color = AtmosToVulkanColor(imageRender.color);

        const auto adjustedSliceLeft = slice.Left() / imageAsset->Width();
        const auto adjustedSliceTop = slice.Top() / imageAsset->Height();
        const auto adjustedSliceRight = slice.Right() / imageAsset->Width();
        const auto adjustedSliceBottom = slice.Bottom() / imageAsset->Height();

        const std::array<Vertex, 4> vertices
        {
            Vertex
            {
                color,
                { position.x - halfWidth, position.y - halfHeight },
                { adjustedSliceLeft, adjustedSliceTop }
            },
            Vertex
            {
                color,
                { position.x + halfWidth, position.y - halfHeight },
                { adjustedSliceRight, adjustedSliceTop }
            },
            Vertex
            {
                color,
                { position.x - halfWidth, position.y + halfHeight },
                { adjustedSliceLeft, adjustedSliceBottom }
            },
            Vertex
            {
                color,
                { position.x + halfWidth, position.y + halfHeight },
                { adjustedSliceRight, adjustedSliceBottom }
            }
        };
        const auto imageAssetData = imageAsset->FileDataAs<ImageAssetDataImplementation>();
        auto context = core.ContextFor(imageRender.position.z);
        if (!context)
            context = &core.AddContext(imageRender.position.z, Context{ imageAssetData->descriptor });
        auto& group = context->GroupFor(*materialAsset);
        group.ListFor(*imageAsset).emplace_back(vertices);
        core.allDiscriminations.emplace(imageAsset);
    }

    void QuadRenderer::Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer)
    {
        core.Start(materials, commandBuffer);
    }

    void QuadRenderer::DrawNextLayer(uint32_t currentImage, glm::vec2 cameraSize)
    {
        const auto setupDiscrimination = [this](const Asset::Image* discrimination, vk::DescriptorSet& descriptorSet)
        {
            const auto imageAssetData = discrimination->FileDataAs<ImageAssetDataImplementation>();
            auto imageAssetDescriptor = imageAssetData->descriptor;
            imageAssetDescriptor.Update(descriptorSet, *device);
        };

        core.AttemptReconstructDiscriminatedDescriptorSet(setupDiscrimination);

        auto drawContext = core.CurrentDrawContext();
        auto& context = drawContext->currentLayer->second;
        for (auto& group : context.groups)
        {
            WriteToBuffers(
                group.second,
                group.first,
                drawContext->commandBuffer,
                currentImage,
                cameraSize);
        }

        ++drawContext->layerCount;
        ++drawContext->currentLayer;
    }

    void QuadRenderer::End()
    {
        core.End();
    }

    bool QuadRenderer::IsDone() const
    {
        return core.IsDone();
    }

    Position3D::Value QuadRenderer::NextLayer() const
    {
        return core.NextLayer();
    }

    size_t QuadRenderer::LayerCount() const
    {
        return core.LayerCount();
    }

    QuadRenderer::Quad::Quad(const Vertices& vertices) : vertices(vertices)
    {}

    auto QuadRenderer::Context::Group::ListFor(const Asset::Image& image) -> std::vector<Quad>&
    {
        auto found = quads.find(&image);
        if (found == quads.end())
            return quads.emplace(&image, std::vector<Quad>{}).first->second;

        return found->second;
    }

    auto QuadRenderer::Context::GroupFor(const Asset::Material& material) -> Group&
    {
        auto found = groups.find(&material);
        if (found == groups.end())
            return groups.emplace(&material, Group{}).first->second;

        return found->second;
    }

    void QuadRenderer::WriteToBuffers(
        const Context::Group& group,
        const Asset::Material* materialAsset,
        vk::CommandBuffer commandBuffer,
        std::uint32_t currentImage,
        glm::vec2 cameraSize)
    {
        auto& pipeline = *core.PipelineFor(*materialAsset);
        pipeline.uniformBuffers[currentImage].PushBytes(UniversalData::Ortho(cameraSize), 0);

        for(auto& object : group.quads)
            WriteToBuffers(
                object.second,
                object.first,
                pipeline,
                commandBuffer,
                currentImage);
    }

    void QuadRenderer::WriteToBuffers(
        const std::vector<Quad>& quads,
        const Asset::Image* imageAsset,
        Pipeline& pipeline,
        vk::CommandBuffer commandBuffer,
        std::uint32_t currentImage)
    {
        auto drawContext = core.CurrentDrawContext();
        const auto startCount = drawContext->count;

        std::vector<Vertex> drawnVertices;
        std::vector<Index> drawnIndices;
        for (auto& quad : quads)
        {
            drawnVertices.insert(drawnVertices.begin(), quad.vertices.begin(), quad.vertices.end());
            const auto offsetQuadIndicesBy = drawContext->count * indexIncrement;
            const Indices offsetQuadIndices =
            {
                Index(offsetQuadIndicesBy + indices[0]),
                Index(offsetQuadIndicesBy + indices[1]),
                Index(offsetQuadIndicesBy + indices[2]),
                Index(offsetQuadIndicesBy + indices[3]),
                Index(offsetQuadIndicesBy + indices[4]),
                Index(offsetQuadIndicesBy + indices[5]),
            };
            drawnIndices.insert(drawnIndices.begin(), offsetQuadIndices.begin(), offsetQuadIndices.end());
            ++drawContext->count;
        }

        vk::Buffer vertexBuffers[] = { vertexBuffer.destination.value.get() };
        vk::DeviceSize offsets[] = { 0 };
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

        commandBuffer.bindIndexBuffer(indexBuffer.destination.value.get(), 0, vk::IndexType::eUint16);

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.value.get());

        auto currentDescriptorSet = core.DiscriminatedDescriptorSetFor(
            [imageAsset, currentImage](const Core::DiscriminatedDescriptorSet& descriptorSet)
            {
                return imageAsset == descriptorSet.discriminator && currentImage == descriptorSet.imageIndex;
            })->descriptorSet;

        commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            pipeline.layout.get(),
            0,
            1,
            &currentDescriptorSet,
            0,
            nullptr);

        const auto vertexOffset = vk::DeviceSize(startCount) * 4 * sizeof Vertex;
        const auto vertexSize = drawnVertices.size() * sizeof Vertex;
        const auto indexOffset = vk::DeviceSize(startCount) * indices.size() * sizeof Index;
        const auto indexSize = drawnIndices.size() * sizeof Index;

        vertexBuffer.PushSourceBytes(drawnVertices.data(), vertexOffset, vertexSize);
        vertexBuffer.CopyFromSourceToDestination(vertexOffset, vertexSize, commandBuffers.Pool(), graphicsQueue);
        indexBuffer.PushSourceBytes(drawnIndices.data(), indexOffset, indexSize);
        indexBuffer.CopyFromSourceToDestination(indexOffset, indexSize, commandBuffers.Pool(), graphicsQueue);
        drawContext->commandBuffer.drawIndexed(quads.size() * indices.size(), 1, startCount * indices.size(), 0, 0);
    }
}
