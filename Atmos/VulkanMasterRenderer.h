#pragma once

#include "VulkanIncludes.h"
#include "VulkanCommandBufferPool.h"
#include "VulkanUniversalDataBuffer.h"

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

#include "Logger.h"

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
            Logging::Logger& logger);
        ~MasterRenderer();

        void Initialize(
            vk::SwapchainKHR swapchain,
            std::vector<vk::Image> swapchainImages,
            std::vector<vk::ImageView> swapchainImageViews,
            vk::Format imageFormat,
            vk::Extent2D swapchainExtent,
            Arca::Reliquary& reliquary);
    public:
        void StageRender(const ImageRender& imageRender);
        void StageRender(const LineRender& lineRender);
        void StageRender(const RegionRender& regionRender);

        void DrawFrame(
            const Spatial::ScreenSize& screenSize,
            const Spatial::ScreenPoint& mapPosition);

        void WaitForIdle() const;
    public:
        void OnMaterialCreated(const Arca::Index<Asset::ImageMaterial>& material);
        void OnMaterialCreated(const Arca::Index<Asset::LineMaterial>& material);
        void OnMaterialCreated(const Arca::Index<Asset::RegionMaterial>& material);
        void OnMaterialDestroying(const Arca::Index<Asset::ImageMaterial>& material);
        void OnMaterialDestroying(const Arca::Index<Asset::LineMaterial>& material);
        void OnMaterialDestroying(const Arca::Index<Asset::RegionMaterial>& material);
    private:
        std::shared_ptr<vk::Device> device;
    private:
        static const int maxFramesInFlight = 2;
        size_t currentFrame = 0;
        size_t previousFrame = 1;
    private:
        using IterableRenderers = std::vector<RendererBase*>;

        struct RendererGroup
        {
            QuadRenderer quad;
            LineRenderer line;
            RegionRenderer region;
            RendererGroup(
                std::shared_ptr<vk::Device> device,
                vk::Queue graphicsQueue,
                vk::PhysicalDeviceMemoryProperties memoryProperties,
                vk::RenderPass renderPass,
                vk::Extent2D swapchainExtent,
                const Arca::Batch<Asset::ImageMaterial>& imageMaterials,
                const Arca::Batch<Asset::LineMaterial>& lineMaterials,
                const Arca::Batch<Asset::RegionMaterial>& regionMaterials);
            RendererGroup(RendererGroup&& arg) noexcept = default;

            [[nodiscard]] IterableRenderers AsIterable();
        };
        using RendererGroups = std::list<RendererGroup>;
        RendererGroups rendererGroups;
        RendererGroups::iterator currentRendererGroup;

        [[nodiscard]] bool AllEmpty(const std::vector<RendererBase*>& check) const;
    private:
        std::vector<vk::CommandBuffer> usedCommandBuffers;

        void Draw(
            RendererGroup& rendererGroup,
            UniversalData universalData,
            vk::Framebuffer framebuffer,
            vk::CommandBuffer commandBuffer);
        static void ClearImage(vk::Image image, std::array<float, 4> color, vk::CommandBuffer commandBuffer);
    private:
        vk::SwapchainKHR swapchain;
        vk::Extent2D swapchainExtent;
        vk::UniqueRenderPass renderPass;
        std::vector<vk::UniqueFramebuffer> framebuffers;

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
        UniversalDataBuffer universalDataBuffer;
    private:
        std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
        std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
        std::vector<vk::UniqueFence> inFlightFences;
        std::vector<vk::Fence> imagesInFlight;

        [[nodiscard]] static std::vector<vk::UniqueSemaphore> CreateSemaphores(vk::Device device, size_t count);
        [[nodiscard]] static std::vector<vk::UniqueFence> CreateFences(vk::Device device, size_t count);
    private:
        Logging::Logger* logger;
    };
}