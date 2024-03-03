#pragma once

#include "VulkanIncludes.h"
#include "VulkanCommandBufferPool.h"

#include "VulkanQuadRenderer.h"
#include "VulkanLineRenderer.h"
#include "VulkanRegionRenderer.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ImageRender.h"
#include "LineRender.h"
#include "RegionRender.h"

#include "ScreenSize.h"
#include "ScreenPoint.h"

#include "Event.h"

#undef CreateSemaphore

namespace Atmos::Render::Vulkan
{
    class MasterRenderer final
    {
    public:
        Event<> onOutOfDate;
    public:
        MasterRenderer(
            std::shared_ptr<vk::Device> device,
            vk::Sampler sampler,
            vk::Queue graphicsQueue,
            vk::Queue presentQueue,
            uint32_t graphicsQueueIndex,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            Arca::Reliquary& reliquary);
        ~MasterRenderer();

        void Initialize(
            vk::SwapchainKHR swapchain,
            std::vector<vk::Image> swapchainImages,
            std::vector<vk::ImageView> swapchainImageViews,
            vk::Format imageFormat,
            vk::Extent2D swapchainExtent);
    public:
        void StageRender(const ImageRender& imageRender);
        void StageRender(const LineRender& lineRender);
        void StageRender(const RegionRender& regionRender);

        void DrawFrame(
            const Spatial::ScreenSize& screenSize,
            const Spatial::ScreenPoint& mapPosition);

        void WaitForIdle() const;
    private:
        std::shared_ptr<vk::Device> device;
    private:
        static const int maxFramesInFlight = 2;
        size_t currentFrame = 0;
        size_t previousFrame = 1;
    private:
        using IterableRenderers = std::vector<RendererBase*>;
        IterableRenderers iterableRenderers;

        struct Renderers
        {
            QuadRenderer quad;
            LineRenderer line;
            RegionRenderer region;
            Renderers(
                std::shared_ptr<vk::Device> device,
                vk::Queue graphicsQueue,
                vk::PhysicalDeviceMemoryProperties memoryProperties,
                vk::RenderPass renderPass,
                uint32_t swapchainImageCount,
                vk::Extent2D swapchainExtent,
                const std::vector<const Asset::Material*>& materials);
            Renderers(Renderers&& arg) noexcept = default;

            [[nodiscard]] IterableRenderers AsIterable();
        };
        std::optional<Renderers> renderers;

        [[nodiscard]] bool AllEmpty(const std::vector<RendererBase*>& check) const;
    private:
        void Draw(
            vk::CommandBuffer commandBuffer,
            uint32_t currentSwapchainImage,
            UniversalData universalData);
        static void ClearImage(vk::Image image, std::array<float, 4> color, vk::CommandBuffer commandBuffer);
    private:
        vk::SwapchainKHR swapchain;

        std::vector<vk::Image> swapchainImages;
        std::vector<vk::ImageView> swapchainImageViews;

        vk::UniqueRenderPass renderPass;
        std::vector<vk::UniqueFramebuffer> framebuffers;
        vk::Extent2D swapchainExtent;
    private:
        [[nodiscard]] static vk::UniqueRenderPass CreateRenderPass(
            vk::Device device, vk::Format swapchainImageFormat);

        [[nodiscard]] static std::vector<vk::UniqueFramebuffer> CreateFramebuffers(
            vk::Device device, const std::vector<vk::ImageView>& imageViews, vk::RenderPass renderPass, vk::Extent2D extent);
    private:
        vk::Sampler sampler;

        vk::PhysicalDeviceMemoryProperties memoryProperties;
    private:
        uint32_t graphicsQueueIndex;

        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        CommandBufferPool commandBuffers;
    private:
        std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
        std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
        std::vector<vk::UniqueFence> inFlightFences;
        std::vector<vk::Fence> imagesInFlight;

        [[nodiscard]] static std::vector<vk::UniqueSemaphore> CreateSemaphores(vk::Device device, size_t count);
        [[nodiscard]] static std::vector<vk::UniqueFence> CreateFences(vk::Device device, size_t count);
    private:
        Arca::Reliquary* reliquary;
        void OnMaterialCreated(const Arca::CreatedKnown<Asset::Material>& signal);
        void OnMaterialDestroying(const Arca::DestroyingKnown<Asset::Material>& signal);
    };
}