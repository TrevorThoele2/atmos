#include "VulkanRenderer.h"

#include "VulkanImageAssetData.h"

#include "VulkanUtilities.h"
#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    Renderer::Renderer(
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
        sampler(sampler),
        memoryProperties(memoryProperties),
        graphicsQueue(graphicsQueue),
        presentQueue(presentQueue),
        commandBuffers(*device, graphicsQueueIndex)
    {
        imageAvailableSemaphores = CreateSemaphores(*device, maxFramesInFlight);
        renderFinishedSemaphores = CreateSemaphores(*device, maxFramesInFlight);

        inFlightFences = CreateFences(*device, maxFramesInFlight);

        allRenderers = { &quadRenderer, &lineRenderer };
    }

    Renderer::~Renderer()
    {
        device->waitIdle();

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

        imagesInFlight.resize(images.size(), nullptr);

        quadRenderer.Initialize(images.size(), renderPass.get(), extent);
        lineRenderer.Initialize(images.size(), renderPass.get(), extent);
    }

    void Renderer::StageRender(const ImageRender& imageRender)
    {
        quadRenderer.StageRender(imageRender);
    }

    void Renderer::StageRender(const LineRender& lineRender)
    {
        lineRenderer.StageRender(lineRender);
    }

    void Renderer::DrawFrame(Arca::Reliquary& reliquary, const ScreenSize& cameraSize)
    {
        device->waitForFences(inFlightFences[previousFrame].get(), VK_TRUE, UINT64_MAX);

        if (quadRenderer.LayerCount() == 0 && lineRenderer.LayerCount() == 0)
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
            if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR)
                onOutOfDate();
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

    void Renderer::Draw(
        Arca::Reliquary& reliquary,
        std::vector<vk::CommandBuffer>& usedCommandBuffers,
        std::uint32_t currentImage,
        glm::vec2 cameraSize)
    {
        if (quadRenderer.LayerCount() == 0 && lineRenderer.LayerCount() == 0)
            return;

        const vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4> { 0.0f, 0.0f, 0.0f, 1.0f }));
        const vk::RenderPassBeginInfo renderPassBeginInfo(
            renderPass.get(),
            framebuffers[currentImage].get(),
            vk::Rect2D({ 0, 0 }, extent),
            1,
            &clearValue);

        const auto commandBuffer = commandBuffers.Next();
        usedCommandBuffers.push_back(commandBuffer);

        const vk::CommandBufferBeginInfo beginInfo({}, nullptr);
        commandBuffer.begin(beginInfo);

        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        quadRenderer.Start(reliquary, commandBuffer);
        lineRenderer.Start(reliquary, commandBuffer);

        const auto end = [&]()
        {
            quadRenderer.End();
            lineRenderer.End();

            commandBuffer.endRenderPass();

            commandBuffer.end();
        };

        try
        {
            auto availableRenderers = allRenderers;
            while (true)
            {
                AllRenderers::iterator nextRenderer;
                for (auto checkRenderer = allRenderers.begin(); checkRenderer != allRenderers.end(); ++checkRenderer)
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

    void Renderer::ClearImage(vk::Image image, std::array<float, 4> color, vk::CommandBuffer commandBuffer)
    {
        const vk::ImageSubresourceRange subresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };

        const vk::ClearColorValue clearColor(color);
        commandBuffer.clearColorImage(image, vk::ImageLayout::eUndefined, clearColor, subresourceRange);
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
