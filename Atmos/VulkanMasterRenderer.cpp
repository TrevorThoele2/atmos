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
        vk::PhysicalDeviceMemoryProperties memoryProperties)
        :
        device(device),
        quadRenderer(device, graphicsQueueIndex, graphicsQueue, memoryProperties),
        lineRenderer(device, graphicsQueueIndex, graphicsQueue, memoryProperties),
        regionRenderer(device, graphicsQueueIndex, graphicsQueue, memoryProperties),
        sampler(sampler),
        memoryProperties(memoryProperties),
        graphicsQueue(graphicsQueue),
        presentQueue(presentQueue),
        commandBuffers(*device, graphicsQueueIndex)
    {
        imageAvailableSemaphores = CreateSemaphores(*device, maxFramesInFlight);
        renderFinishedSemaphores = CreateSemaphores(*device, maxFramesInFlight);

        inFlightFences = CreateFences(*device, maxFramesInFlight);

        allRenderers = { &quadRenderer, &lineRenderer, &regionRenderer };
    }

    MasterRenderer::~MasterRenderer()
    {
        device->waitIdle();

        inFlightFences.clear();
    }

    void MasterRenderer::Initialize(
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

        imagesInFlight.resize(images.size(), nullptr);

        for (auto& renderer : allRenderers)
            renderer->Initialize(images.size(), renderPass.get(), extent);
    }

    void MasterRenderer::StageRender(const ImageRender& imageRender)
    {
        quadRenderer.StageRender(imageRender);
    }

    void MasterRenderer::StageRender(const LineRender& lineRender)
    {
        lineRenderer.StageRender(lineRender);
    }

    void MasterRenderer::StageRender(const RegionRender& regionRender)
    {
        regionRenderer.StageRender(regionRender);
    }

    void MasterRenderer::DrawFrame(Arca::Reliquary& reliquary, const ScreenSize& cameraSize)
    {
        device->waitForFences(inFlightFences[previousFrame].get(), VK_TRUE, UINT64_MAX);

        if (AllEmpty(allRenderers))
            return;

        auto imageIndex = device->acquireNextImageKHR(
            swapchain,
            UINT64_MAX,
            imageAvailableSemaphores[currentFrame].get(),
            nullptr);
        if (imageIndex.result == vk::Result::eErrorOutOfDateKHR)
        {
            Logging::logger.Log("Vulkan image is out of date.");
            onOutOfDate();
            return;
        }
        else if (IsError(imageIndex.result) && imageIndex.result != vk::Result::eSuboptimalKHR)
            throw GraphicsError("Could not acquire next image in swapchain.");

        if (imagesInFlight[imageIndex.value])
            device->waitForFences(1, &imagesInFlight[imageIndex.value], VK_TRUE, UINT64_MAX);
        imagesInFlight[imageIndex.value] = inFlightFences[currentFrame].get();

        commandBuffers.Reserve(images.size());

        std::vector<vk::CommandBuffer> usedCommandBuffers;

        const auto endFrame = [&]()
        {
            for (auto& buffer : usedCommandBuffers)
                commandBuffers.DoneWith(buffer);

            previousFrame = currentFrame;
            currentFrame = (currentFrame + 1) % maxFramesInFlight;
        };

        try
        {
            Draw(
                reliquary,
                usedCommandBuffers,
                imageIndex.value,
                glm::vec2{ cameraSize.width, cameraSize.height });

            vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame].get() };
            vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame].get() };
            vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
            vk::SubmitInfo submitInfo(
                1,
                waitSemaphores,
                waitStages,
                usedCommandBuffers.size(),
                usedCommandBuffers.data(),
                1,
                signalSemaphores);

            device->resetFences(1, &inFlightFences[currentFrame].get());

            graphicsQueue.submit(1, &submitInfo, inFlightFences[currentFrame].get());

            const vk::SwapchainKHR swapchains[] = { swapchain };
            vk::PresentInfoKHR presentInfo(1, signalSemaphores, 1, swapchains, &imageIndex.value, nullptr);
            const auto presentResult = presentQueue.presentKHR(&presentInfo);
            if (presentResult == vk::Result::eErrorOutOfDateKHR)
            {
                Logging::logger.Log("Vulkan presentation is out of date.", Logging::Severity::Verbose);
                onOutOfDate();
            }
            else if (presentResult == vk::Result::eSuboptimalKHR)
            {
                Logging::logger.Log("Vulkan presentation is suboptimal.", Logging::Severity::Verbose);
                onOutOfDate();
            }
            else if (IsError(presentResult))
                throw GraphicsError("Could not present swap chain.");
        }
        catch(...)
        {
            device->resetFences(1, &inFlightFences[currentFrame].get());
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
        device->waitForFences(fences, VK_TRUE, UINT64_MAX);
    }

    bool MasterRenderer::AllEmpty(const std::vector<RendererInterface*>& check) const
    {
        for (auto& renderer : check)
            if (renderer->LayerCount() != 0)
                return false;

        return true;
    }

    void MasterRenderer::Draw(
        Arca::Reliquary& reliquary,
        std::vector<vk::CommandBuffer>& usedCommandBuffers,
        uint32_t currentImage,
        glm::vec2 cameraSize)
    {
        const auto commandBuffer = commandBuffers.Next();
        usedCommandBuffers.push_back(commandBuffer);

        const auto materialBatch = reliquary.Batch<Asset::Material>();
        std::vector<const Asset::Material*> materials;
        materials.reserve(materialBatch.Size());
        for (auto& material : materialBatch)
            materials.push_back(&material);

        std::vector<RendererInterface*> startedRenderers;
        for (auto& renderer : allRenderers)
        {
            if (renderer->LayerCount() != 0)
            {
                startedRenderers.push_back(renderer);
                renderer->Start(materials, commandBuffer);
            }
        }

        const vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4> { 0.0f, 0.0f, 0.0f, 1.0f }));
        const vk::RenderPassBeginInfo renderPassBeginInfo(
            renderPass.get(),
            framebuffers[currentImage].get(),
            vk::Rect2D({ 0, 0 }, extent),
            1,
            &clearValue);

        const vk::CommandBufferBeginInfo beginInfo({}, nullptr);
        commandBuffer.begin(beginInfo);

        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        const auto end = [&]()
        {
            for (auto& renderer : startedRenderers)
                renderer->End();

            commandBuffer.endRenderPass();

            commandBuffer.end();
        };

        try
        {
            auto availableRenderers = startedRenderers;
            while (true)
            {
                AllRenderers::iterator nextRenderer;
                for (auto checkRenderer = startedRenderers.begin(); checkRenderer != startedRenderers.end(); ++checkRenderer)
                {
                    if (!(*checkRenderer)->IsDone())
                    {
                        nextRenderer = checkRenderer;
                        break;
                    }
                }

                for(auto checkRenderer = availableRenderers.begin(); checkRenderer != availableRenderers.end();)
                {
                    if ((*checkRenderer)->IsDone())
                        checkRenderer = availableRenderers.erase(checkRenderer);
                    else 
                    {
                        if ((*checkRenderer)->NextLayer() < (*nextRenderer)->NextLayer())
                            nextRenderer = checkRenderer;
                        ++checkRenderer;
                    }
                }

                if (availableRenderers.empty())
                    break;

                (*nextRenderer)->DrawNextLayer(currentImage, cameraSize);
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
