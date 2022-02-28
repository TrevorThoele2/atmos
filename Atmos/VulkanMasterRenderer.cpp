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
    
    void MasterRenderer::DrawFrame(
        const std::vector<Render::Raster::Command>& commands,
        const Spatial::Size2D& screenSize,
        const Spatial::Point2D& mapPosition)
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
                commands,
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
    
    MasterRenderer::RendererGroup::RendererGroup(
        vk::Device device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        GlyphAtlas& glyphAtlas)
        :
        quad(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainExtent,
            glyphAtlas),
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
            swapchainExtent)
    {}
    
    void MasterRenderer::Draw(
        const std::vector<Raster::Command>& commands,
        RendererGroup& rendererGroup,
        UniversalData universalData,
        vk::Framebuffer framebuffer,
        vk::CommandBuffer commandBuffer)
    {
        if (commands.empty())
            return;

        universalDataBuffer.PushBytes(universalData);

        constexpr vk::CommandBufferBeginInfo beginInfo({}, nullptr);
        commandBuffer.begin(beginInfo);
        
        try
        {
            std::vector<Command> writeDataCommands;
            std::vector<Command> drawCommands;

            const auto renderGroup = [&](const auto& draws, const auto rendererSelector)
            {
                const auto renderedCommands = (rendererGroup.*rendererSelector).Draw(draws, universalDataBuffer);
                writeDataCommands.push_back(renderedCommands->writeData);
                drawCommands.push_back(renderedCommands->draw);
            };
            
            for (auto command = commands.begin(); command != commands.end();)
            {
                if (std::holds_alternative<Raster::DrawImage>(*command))
                    renderGroup(GroupDrawCommands<Raster::DrawImage>(command, commands), &RendererGroup::quad);
                else if (std::holds_alternative<Raster::DrawLine>(*command))
                    renderGroup(GroupDrawCommands<Raster::DrawLine>(command, commands), &RendererGroup::line);
                else if (std::holds_alternative<Raster::DrawRegion>(*command))
                    renderGroup(GroupDrawCommands<Raster::DrawRegion>(command, commands), &RendererGroup::region);
                else if (std::holds_alternative<Raster::DrawText>(*command))
                    renderGroup(GroupDrawCommands<Raster::DrawText>(command, commands), &RendererGroup::quad);
            }
            
            auto recorder = CommandRecorder(commandBuffer);
            for (auto& command : writeDataCommands)
                recorder(command);

            const auto clearValue = vk::ClearValue(vk::ClearColorValue(std::array{ 0.0f, 0.0f, 0.0f, 1.0f }));
            RenderPass(
                vk::RenderPassBeginInfo(renderPass.get(), framebuffer, vk::Rect2D({ 0, 0 }, swapchainExtent), 1, &clearValue),
                vk::SubpassContents::eInline,
                commandBuffer,
                [&drawCommands, &recorder]
                {
                    for (auto& command : drawCommands)
                        recorder(command);
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

        constexpr vk::AttachmentReference colorAttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);

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