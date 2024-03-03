#include "VulkanMasterRenderer.h"

#include "VulkanRenderPass.h"
#include "VulkanSynchronization.h"
#include "VulkanResults.h"
#include "VulkanMaxFramesInFlight.h"

#include "GraphicsError.h"
#include "Logger.h"

namespace Atmos::Render::Vulkan
{
    MasterRenderer::MasterRenderer(
        vk::Device device,
        vk::Sampler sampler,
        vk::Queue graphicsQueue,
        vk::Queue presentQueue,
        uint32_t graphicsQueueIndex,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        GlyphAtlas& glyphAtlas,
        Logging::Logger& logger)
        :
        device(device),
        sampler(sampler),
        memoryProperties(memoryProperties),
        graphicsQueue(graphicsQueue),
        presentQueue(presentQueue),
        drawCommandBuffers(device, graphicsQueueIndex, vk::CommandPoolCreateFlagBits::eResetCommandBuffer),
        memoryPool(0, memoryProperties, device),
        universalDataBuffer(0, memoryPool, device),
        glyphAtlas(&glyphAtlas),
        logger(&logger)
    {
        imageAvailableSemaphores = CreateSemaphores(device, maxFramesInFlight);
        renderFinishedSemaphores = CreateSemaphores(device, maxFramesInFlight);

        inFlightFences = CreateFences(device, maxFramesInFlight);
    }

    MasterRenderer::~MasterRenderer()
    {
        device.waitIdle();

        inFlightFences.clear();
    }

    void MasterRenderer::Initialize(
        vk::SwapchainKHR swapchain,
        std::vector<vk::Image> swapchainImages,
        std::vector<vk::ImageView> swapchainImageViews,
        vk::Format imageFormat,
        vk::Extent2D swapchainExtent)
    {
        this->swapchain = swapchain;
        this->swapchainExtent = swapchainExtent;

        renderPass = CreateRenderPass(device, imageFormat);

        framebuffers = CreateFramebuffers(
            device, swapchainImageViews, renderPass.get(), swapchainExtent);

        swapchainImagesInFlight.resize(swapchainImages.size(), nullptr);

        drawCommandBuffers.Reserve(swapchainImages.size());
        
        rendererGroups.clear();
        for (uint32_t i = 0; i < swapchainImages.size(); ++i)
            rendererGroups.emplace_back(
                device,
                graphicsQueue,
                memoryProperties,
                renderPass.get(),
                swapchainExtent,
                *glyphAtlas);
        currentRendererGroup = rendererGroups.begin();
    }

    void MasterRenderer::StageRender(const RenderImage& imageRender)
    {
        currentRendererGroup->image.StageRender(imageRender);
    }

    void MasterRenderer::StageRender(const RenderLine& lineRender)
    {
        currentRendererGroup->line.StageRender(lineRender);
    }

    void MasterRenderer::StageRender(const RenderRegion& regionRender)
    {
        currentRendererGroup->region.StageRender(regionRender);
    }

    void MasterRenderer::StageRender(const RenderText& textRender)
    {
        currentRendererGroup->text.StageRender(textRender);
    }

    void MasterRenderer::DrawFrame(const Spatial::Size2D& screenSize, const Spatial::Point2D& mapPosition)
    {
        if (IsError(device.waitForFences(inFlightFences[previousFrame].get(), VK_TRUE, UINT64_MAX)))
            logger->Log("Could not wait for Vulkan fences.");

        const auto swapchainImageIndex = device.acquireNextImageKHR(
            swapchain,
            UINT64_MAX,
            imageAvailableSemaphores[currentFrame].get(),
            nullptr);
        if (swapchainImageIndex.result == vk::Result::eErrorOutOfDateKHR)
        {
            logger->Log("Vulkan swapchain image is out of date.");
            onOutOfDate();
            return;
        }
        else if (IsError(swapchainImageIndex.result) && swapchainImageIndex.result != vk::Result::eSuboptimalKHR)
            throw GraphicsError("Could not acquire next image in swapchain.");

        const auto currentSwapchainImage = swapchainImageIndex.value;

        if (swapchainImagesInFlight[currentSwapchainImage])
            if (IsError(device.waitForFences(1, &swapchainImagesInFlight[currentSwapchainImage], VK_TRUE, UINT64_MAX)))
                logger->Log("Could not wait for Vulkan fences.");
        swapchainImagesInFlight[currentSwapchainImage] = inFlightFences[currentFrame].get();
        
        const auto drawCommandBuffer = drawCommandBuffers.Checkout();
        for (const auto& usedCommandBuffer : usedCommandBuffers)
            drawCommandBuffers.Return(usedCommandBuffer);
        usedCommandBuffers.clear();
        usedCommandBuffers.push_back(drawCommandBuffer);

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
                drawCommandBuffer);

            vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame].get() };
            vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame].get() };
            vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

            if (IsError(device.resetFences(1, &inFlightFences[currentFrame].get())))
                logger->Log("Could not reset Vulkan fences.");

            const vk::SubmitInfo submitInfo(
                1,
                waitSemaphores,
                waitStages,
                1,
                &drawCommandBuffer,
                1,
                signalSemaphores);

            if (IsError(graphicsQueue.submit(1, &submitInfo, inFlightFences[currentFrame].get())))
                logger->Log("Could not submit graphics queue.");

            const vk::SwapchainKHR swapchains[] = { swapchain };
            const vk::PresentInfoKHR presentInfo(1, signalSemaphores, 1, swapchains, &swapchainImageIndex.value, nullptr);
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
            if (IsError(device.resetFences(1, &inFlightFences[currentFrame].get())))
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
        if (IsError(device.waitForFences(fences, VK_TRUE, UINT64_MAX)))
            logger->Log("Could not wait for Vulkan fences.");
    }
    
    void MasterRenderer::OnMaterialDestroying(const Arca::Index<Asset::Material>& material)
    {
        for (auto& group : rendererGroups)
        {
            group.image.MaterialDestroying(material);
            group.line.MaterialDestroying(material);
            group.region.MaterialDestroying(material);
            group.text.MaterialDestroying(material);
        }
    }
    
    MasterRenderer::RendererGroup::RendererGroup(
        vk::Device device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        GlyphAtlas& glyphAtlas)
        :
        image(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainExtent),
        line(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainExtent),
        region(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainExtent),
        text(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainExtent,
            glyphAtlas)
    {}

    auto MasterRenderer::RendererGroup::AsIterable() -> IterableRenderers
    {
        return { &image, &line, &region, &text };
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

        using Rasters = std::vector<std::shared_ptr<Raster>>;
        Rasters rasters;
        for (const auto& renderer : rendererGroup.AsIterable())
            if (renderer->RenderCount() > 0)
                rasters.push_back(renderer->Start(commandBuffer, universalDataBuffer));
        
        try
        {
            auto availableRasters = rasters;
            std::vector<Raster::Pass> rasterPasses;
            while (true)
            {
                std::erase_if(availableRasters, [](auto raster) { return raster->IsDone(); });
                if (availableRasters.empty())
                    break;

                const auto raster = *std::ranges::min_element(availableRasters, [](auto left, auto right) { return left->NextLayer() < right->NextLayer(); });
                const auto nextRasterPasses = raster->NextPasses();
                rasterPasses.insert(rasterPasses.end(), nextRasterPasses.begin(), nextRasterPasses.end());
            }

            auto recorder = CommandRecorder(commandBuffer);
            for (auto& rasterPass : rasterPasses)
                recorder(rasterPass.writeData);

            const auto clearValue = vk::ClearValue(vk::ClearColorValue(std::array{ 0.0f, 0.0f, 0.0f, 1.0f }));
            RenderPass(
                vk::RenderPassBeginInfo(renderPass.get(), framebuffer, vk::Rect2D({ 0, 0 }, swapchainExtent), 1, &clearValue),
                vk::SubpassContents::eInline,
                commandBuffer,
                [&rasterPasses, commandBuffer]
                {
                    auto recorder = CommandRecorder(commandBuffer);
                    for (auto& rasterPass : rasterPasses)
                        recorder(rasterPass.draw);
                });
        }
        catch(...)
        {
            commandBuffer.end();

            throw;
        }

        commandBuffer.end();
    }

    vk::UniqueRenderPass MasterRenderer::CreateRenderPass(
        vk::Device device, vk::Format swapchainImageFormat)
    {
        const vk::AttachmentDescription colorAttachment(
            {},
            swapchainImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR);

        const vk::AttachmentReference colorAttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);

        const vk::SubpassDescription subpass(
            {},
            vk::PipelineBindPoint::eGraphics,
            {},
            {},
            1,
            &colorAttachmentReference);

        const vk::SubpassDependency dependency(
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
        
        for (size_t i = 0; i < count; ++i)
            returnValue.push_back(CreateSemaphore(device, {}));

        return returnValue;
    }

    std::vector<vk::UniqueFence> MasterRenderer::CreateFences(vk::Device device, size_t count)
    {
        std::vector<vk::UniqueFence> returnValue;
        
        for (size_t i = 0; i < count; ++i)
            returnValue.push_back(CreateFence(device, vk::FenceCreateFlagBits::eSignaled));

        return returnValue;
    }
}