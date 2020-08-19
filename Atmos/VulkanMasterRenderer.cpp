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
        Arca::Reliquary& reliquary)
        :
        device(device),
        sampler(sampler),
        memoryProperties(memoryProperties),
        graphicsQueueIndex(graphicsQueueIndex),
        graphicsQueue(graphicsQueue),
        presentQueue(presentQueue),
        commandBuffers(*device, graphicsQueueIndex),
        reliquary(&reliquary)
    {
        imageAvailableSemaphores = CreateSemaphores(*device, maxFramesInFlight);
        renderFinishedSemaphores = CreateSemaphores(*device, maxFramesInFlight);

        inFlightFences = CreateFences(*device, maxFramesInFlight);

        reliquary.On<Arca::CreatedKnown<Asset::Material>>(
            [this](const Arca::CreatedKnown<Asset::Material>& signal) { OnMaterialCreated(signal); });
        reliquary.On<Arca::DestroyingKnown<Asset::Material>>(
            [this](const Arca::DestroyingKnown<Asset::Material>& signal) { OnMaterialDestroying(signal); });
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
        vk::Extent2D swapchainExtent)
    {
        this->swapchain = swapchain;
        this->swapchainImages = swapchainImages;
        this->swapchainImageViews = swapchainImageViews;
        this->swapchainExtent = swapchainExtent;

        renderPass = CreateRenderPass(*device, imageFormat);

        framebuffers = CreateFramebuffers(
            *device, swapchainImageViews, renderPass.get(), swapchainExtent);

        imagesInFlight.resize(swapchainImages.size(), nullptr);

        const auto materialBatch = reliquary->Batch<Asset::Material>();
        std::vector<const Asset::Material*> materials;
        materials.reserve(materialBatch.Size());
        for (auto& material : materialBatch)
            materials.push_back(&material);

        renderers.emplace(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass.get(),
            swapchainImages.size(),
            swapchainExtent,
            materials);
        iterableRenderers = renderers->AsIterable();
    }

    void MasterRenderer::StageRender(const ImageRender& imageRender)
    {
        renderers->quad.StageRender(imageRender);
    }

    void MasterRenderer::StageRender(const LineRender& lineRender)
    {
        renderers->line.StageRender(lineRender);
    }

    void MasterRenderer::StageRender(const RegionRender& regionRender)
    {
        renderers->region.StageRender(regionRender);
    }

    void MasterRenderer::DrawFrame(
        const Spatial::ScreenSize& screenSize,
        const Spatial::ScreenPoint& mapPosition)
    {
        device->waitForFences(inFlightFences[previousFrame].get(), VK_TRUE, UINT64_MAX);

        if (AllEmpty(iterableRenderers))
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

        commandBuffers.Reserve(swapchainImages.size());

        auto commandBuffer = commandBuffers.Next();

        const auto endFrame = [&]()
        {
            commandBuffers.DoneWith(commandBuffer);

            previousFrame = currentFrame;
            currentFrame = (currentFrame + 1) % maxFramesInFlight;
        };

        try
        {
            const auto universalData = UniversalData(
                glm::vec2{ screenSize.width, screenSize.height },
                glm::vec2{ mapPosition.x, mapPosition.y });

            Draw(commandBuffer, imageIndex.value, universalData);

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

    MasterRenderer::Renderers::Renderers(
        std::shared_ptr<vk::Device> device,
        vk::Queue graphicsQueue,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::RenderPass renderPass,
        uint32_t swapchainImageCount,
        vk::Extent2D swapchainExtent,
        const std::vector<const Asset::Material*>& materials)
        :
        quad(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainImageCount,
            swapchainExtent,
            materials),
        line(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainImageCount,
            swapchainExtent,
            materials),
        region(
            device,
            graphicsQueue,
            memoryProperties,
            renderPass,
            swapchainImageCount,
            swapchainExtent,
            materials)
    {}

    auto MasterRenderer::Renderers::AsIterable() -> IterableRenderers
    {
        return { &quad, &line, &region };
    }

    bool MasterRenderer::AllEmpty(const std::vector<RendererBase*>& check) const
    {
        for (auto& renderer : check)
            if (renderer->LayerCount() != 0)
                return false;

        return true;
    }

    void MasterRenderer::Draw(
        vk::CommandBuffer commandBuffer,
        uint32_t currentSwapchainImage,
        UniversalData universalData)
    {
        const vk::CommandBufferBeginInfo beginInfo({}, nullptr);
        commandBuffer.begin(beginInfo);

        const vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4> { 0.0f, 0.0f, 0.0f, 1.0f }));
        const vk::RenderPassBeginInfo renderPassBeginInfo(
            renderPass.get(),
            framebuffers[currentSwapchainImage].get(),
            vk::Rect2D({ 0, 0 }, swapchainExtent),
            1,
            &clearValue);
        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        std::vector<RendererBase*> startedRenderers;
        for (auto& renderer : iterableRenderers)
        {
            if (renderer->LayerCount() > 0)
            {
                startedRenderers.push_back(renderer);
                renderer->Start(commandBuffer, commandBuffers.Pool(), currentSwapchainImage, universalData);
            }
        }

        const auto end = [&startedRenderers, &commandBuffer]()
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
                IterableRenderers::iterator nextRenderer;
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

                (*nextRenderer)->DrawNextLayer();
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

    void MasterRenderer::OnMaterialCreated(const Arca::CreatedKnown<Asset::Material>& signal)
    {
        for (auto& renderer : iterableRenderers)
            renderer->MaterialCreated(*signal.reference);
    }

    void MasterRenderer::OnMaterialDestroying(const Arca::DestroyingKnown<Asset::Material>& signal)
    {
        for (auto& renderer : iterableRenderers)
            renderer->MaterialDestroying(*signal.reference);
    }
}
