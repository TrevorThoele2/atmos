#include "VulkanMasterRenderer.h"

#include "VulkanUtilities.h"
#include "GraphicsError.h"
#include "Logger.h"

namespace Atmos::Render::Vulkan
{
    MasterRenderer::MasterRenderer(
        std::shared_ptr<vk::Device> device,
        vk::Sampler sampler,
        vk::Queue graphicsQueue,
        vk::Queue presentQueue,
        uint32_t graphicsQueueIndex,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        Logging::Logger& logger)
        :
        device(device),
        sampler(sampler),
        memoryProperties(memoryProperties),
        graphicsQueueIndex(graphicsQueueIndex),
        graphicsQueue(graphicsQueue),
        presentQueue(presentQueue),
        commandBuffers(*device, graphicsQueueIndex),
        universalDataBuffer(0, memoryProperties, *device),
        logger(&logger)
    {
        imageAvailableSemaphores = CreateSemaphores(*device, maxFramesInFlight);
        renderFinishedSemaphores = CreateSemaphores(*device, maxFramesInFlight);

        inFlightFences = CreateFences(*device, maxFramesInFlight);
    }

    MasterRenderer::~MasterRenderer()
    {
        device->waitIdle();

        inFlightFences.clear();
    }

    void MasterRenderer::Initialize(
        vk::SwapchainKHR swapchain,
        std::vector<vk::Image> swapchainImages,
        std::vector<vk::ImageView> swapchainImageViews,
        vk::Format imageFormat,
        vk::Extent2D swapchainExtent,
        Arca::Reliquary& reliquary)
    {
        this->swapchain = swapchain;
        this->swapchainExtent = swapchainExtent;

        renderPass = CreateRenderPass(*device, imageFormat);

        framebuffers = CreateFramebuffers(
            *device, swapchainImageViews, renderPass.get(), swapchainExtent);

        imagesInFlight.resize(swapchainImages.size(), nullptr);

        const auto imageMaterialBatch = reliquary.Batch<Asset::ImageMaterial>();
        const auto lineMaterialBatch = reliquary.Batch<Asset::LineMaterial>();
        const auto regionMaterialBatch = reliquary.Batch<Asset::RegionMaterial>();

        rendererGroups.clear();
        for (uint32_t i = 0; i < swapchainImages.size(); ++i)
        {
            rendererGroups.emplace_back(
                device,
                graphicsQueue,
                memoryProperties,
                renderPass.get(),
                swapchainExtent,
                imageMaterialBatch,
                lineMaterialBatch,
                regionMaterialBatch);
        }
        currentRendererGroup = rendererGroups.begin();

        commandBuffers.Reserve(swapchainImages.size());
    }

    void MasterRenderer::StageRender(const ImageRender& imageRender)
    {
        currentRendererGroup->quad.StageRender(imageRender);
    }

    void MasterRenderer::StageRender(const LineRender& lineRender)
    {
        currentRendererGroup->line.StageRender(lineRender);
    }

    void MasterRenderer::StageRender(const RegionRender& regionRender)
    {
        currentRendererGroup->region.StageRender(regionRender);
    }

    void MasterRenderer::DrawFrame(const Spatial::ScreenSize& screenSize, const Spatial::ScreenPoint& mapPosition)
    {
        if (IsError(device->waitForFences(inFlightFences[previousFrame].get(), VK_TRUE, UINT64_MAX)))
            logger->Log("Could not wait for Vulkan fences.");

        auto imageIndex = device->acquireNextImageKHR(
            swapchain,
            UINT64_MAX,
            imageAvailableSemaphores[currentFrame].get(),
            nullptr);
        if (imageIndex.result == vk::Result::eErrorOutOfDateKHR)
        {
            logger->Log("Vulkan image is out of date.");
            onOutOfDate();
            return;
        }
        else if (IsError(imageIndex.result) && imageIndex.result != vk::Result::eSuboptimalKHR)
            throw GraphicsError("Could not acquire next image in swapchain.");

        const auto currentSwapchainImage = imageIndex.value;

        if (imagesInFlight[currentSwapchainImage])
            if (IsError(device->waitForFences(1, &imagesInFlight[currentSwapchainImage], VK_TRUE, UINT64_MAX)))
                logger->Log("Could not wait for Vulkan fences.");
        imagesInFlight[currentSwapchainImage] = inFlightFences[currentFrame].get();

        auto commandBuffer = commandBuffers.Next();
        for (auto& usedCommandBuffer : usedCommandBuffers)
            commandBuffers.DoneWith(usedCommandBuffer);
        usedCommandBuffers.clear();
        usedCommandBuffers.push_back(commandBuffer);

        const auto endFrame = [&]()
        {
            previousFrame = currentFrame;
            currentFrame = (currentFrame + 1) % maxFramesInFlight;

            ++currentRendererGroup;
            if (currentRendererGroup == rendererGroups.end())
                currentRendererGroup = rendererGroups.begin();
        };

        try
        {
            const auto universalData = UniversalData(
                glm::vec2{ screenSize.width, screenSize.height },
                glm::vec2{ mapPosition.x, mapPosition.y });

            Draw(
                *currentRendererGroup,
                universalData,
                framebuffers[currentSwapchainImage].get(),
                commandBuffer);

            vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame].get() };
            vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame].get() };
            vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
            vk::SubmitInfo submitInfo(
                1,
                waitSemaphores,
                waitStages,
                1,
                &commandBuffer,
                1,
                signalSemaphores);

            if (IsError(device->resetFences(1, &inFlightFences[currentFrame].get())))
                logger->Log("Could not reset Vulkan fences.");

            if (IsError(graphicsQueue.submit(1, &submitInfo, inFlightFences[currentFrame].get())))
                logger->Log("Could not submit graphics queue.");

            const vk::SwapchainKHR swapchains[] = { swapchain };
            vk::PresentInfoKHR presentInfo(1, signalSemaphores, 1, swapchains, &imageIndex.value, nullptr);
            const auto presentResult = presentQueue.presentKHR(&presentInfo);
            if (presentResult == vk::Result::eErrorOutOfDateKHR)
            {
                logger->Log("Vulkan presentation is out of date.", Logging::Severity::Verbose);
                onOutOfDate();
            }
            else if (presentResult == vk::Result::eSuboptimalKHR)
            {
                logger->Log("Vulkan presentation is suboptimal.", Logging::Severity::Verbose);
                onOutOfDate();
            }
            else if (IsError(presentResult))
                throw GraphicsError("Could not present swap chain.");
        }
        catch(...)
        {
            if (IsError(device->resetFences(1, &inFlightFences[currentFrame].get())))
                logger->Log("Could not reset Vulkan fences.");
            endFrame();
            throw;
        }

        endFrame();
    }

    void MasterRenderer::WaitForIdle() const
    {
        std::vector<vk::Fence> fences;
        fences.reserve(inFlightFences.size());
        for (auto& fence : inFlightFences)
            fences.push_back(fence.get());
        if (IsError(device->waitForFences(fences, VK_TRUE, UINT64_MAX)))
            logger->Log("Could not wait for Vulkan fences.");
    }

    void MasterRenderer::OnMaterialCreated(const Arca::Index<Asset::ImageMaterial>& material)
    {
        for (auto& group : rendererGroups)
            group.quad.MaterialCreated(material);
    }

    void MasterRenderer::OnMaterialCreated(const Arca::Index<Asset::LineMaterial>& material)
    {
        for (auto& group : rendererGroups)
            group.line.MaterialCreated(material);
    }

    void MasterRenderer::OnMaterialCreated(const Arca::Index<Asset::RegionMaterial>& material)
    {
        for (auto& group : rendererGroups)
            group.region.MaterialCreated(material);
    }

    void MasterRenderer::OnMaterialDestroying(const Arca::Index<Asset::ImageMaterial>& material)
    {
        for (auto& group : rendererGroups)
            group.quad.MaterialDestroying(material);
    }

    void MasterRenderer::OnMaterialDestroying(const Arca::Index<Asset::LineMaterial>& material)
    {
        for (auto& group : rendererGroups)
            group.line.MaterialDestroying(material);
    }

    void MasterRenderer::OnMaterialDestroying(const Arca::Index<Asset::RegionMaterial>& material)
    {
        for (auto& group : rendererGroups)
            group.region.MaterialDestroying(material);
    }

    MasterRenderer::RendererGroup::RendererGroup(
        std::shared_ptr<vk::Device> device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        const Arca::Batch<Asset::ImageMaterial>& imageMaterials,
        const Arca::Batch<Asset::LineMaterial>& lineMaterials,
        const Arca::Batch<Asset::RegionMaterial>& regionMaterials)
        :
        quad(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainExtent,
            imageMaterials),
        line(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainExtent,
            lineMaterials),
        region(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainExtent,
            regionMaterials)
    {}

    auto MasterRenderer::RendererGroup::AsIterable() -> IterableRenderers
    {
        return { &quad, &line, &region };
    }

    bool MasterRenderer::AllEmpty(const std::vector<RendererBase*>& check) const
    {
        for (auto& renderer : check)
            if (renderer->RenderCount() != 0)
                return false;

        return true;
    }

    void MasterRenderer::Draw(
        RendererGroup& rendererGroup,
        UniversalData universalData,
        vk::Framebuffer framebuffer,
        vk::CommandBuffer commandBuffer)
    {
        universalDataBuffer.PushBytes(universalData);

        const vk::CommandBufferBeginInfo beginInfo({}, nullptr);
        commandBuffer.begin(beginInfo);

        const vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4> { 0.0f, 0.0f, 0.0f, 1.0f }));
        const vk::RenderPassBeginInfo renderPassBeginInfo(
            renderPass.get(),
            framebuffer,
            vk::Rect2D({ 0, 0 }, swapchainExtent),
            1,
            &clearValue);
        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        using Rasters = std::vector<std::shared_ptr<Raster>>;
        Rasters startedRasters;
        for (auto& renderer : rendererGroup.AsIterable())
            if (renderer->RenderCount() > 0)
                startedRasters.push_back(
                    renderer->Start(commandBuffer, commandBuffers.Pool(), universalDataBuffer));

        const auto end = [&commandBuffer]()
        {
            commandBuffer.endRenderPass();

            commandBuffer.end();
        };

        try
        {
            auto availableRasters = startedRasters;
            while (true)
            {
                Rasters::iterator nextRaster;
                for (auto checkRaster = startedRasters.begin(); checkRaster != startedRasters.end(); ++checkRaster)
                {
                    if (!(*checkRaster)->IsDone())
                    {
                        nextRaster = checkRaster;
                        break;
                    }
                }

                for(auto checkRaster = availableRasters.begin(); checkRaster != availableRasters.end();)
                {
                    if ((*checkRaster)->IsDone())
                        checkRaster = availableRasters.erase(checkRaster);
                    else 
                    {
                        if ((*checkRaster)->NextLayer() < (*nextRaster)->NextLayer())
                            nextRaster = checkRaster;
                        ++checkRaster;
                    }
                }

                if (availableRasters.empty())
                    break;

                (*nextRaster)->DrawNextLayer();
            }
        }
        catch(...)
        {
            end();
            throw;
        }

        end();
    }

    void MasterRenderer::ClearImage(vk::Image image, std::array<float, 4> color, vk::CommandBuffer commandBuffer)
    {
        const vk::ImageSubresourceRange subresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };

        const vk::ClearColorValue clearColor(color);
        commandBuffer.clearColorImage(image, vk::ImageLayout::eUndefined, clearColor, subresourceRange);
    }

    vk::UniqueRenderPass MasterRenderer::CreateRenderPass(
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

    std::vector<vk::UniqueFramebuffer> MasterRenderer::CreateFramebuffers(
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

    std::vector<vk::UniqueSemaphore> MasterRenderer::CreateSemaphores(vk::Device device, size_t count)
    {
        std::vector<vk::UniqueSemaphore> returnValue;

        const vk::SemaphoreCreateInfo createInfo;
        for (size_t i = 0; i < count; ++i)
            returnValue.push_back(device.createSemaphoreUnique(createInfo));

        return returnValue;
    }

    std::vector<vk::UniqueFence> MasterRenderer::CreateFences(vk::Device device, size_t count)
    {
        std::vector<vk::UniqueFence> returnValue;

        const vk::FenceCreateInfo createInfo(vk::FenceCreateFlagBits::eSignaled);
        for (size_t i = 0; i < count; ++i)
            returnValue.push_back(device.createFenceUnique(createInfo));

        return returnValue;
    }
}