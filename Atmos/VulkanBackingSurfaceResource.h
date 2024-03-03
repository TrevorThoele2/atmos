#pragma once

#include "SurfaceResource.h"
#include "VulkanMasterRenderer.h"
#include "VulkanIncludes.h"

#include "VulkanQueueFamilyIndices.h"
#include "VulkanSwapchainSupport.h"

namespace Atmos::Render::Resource::Vulkan
{
    using namespace Render::Vulkan;

    class BackingSurface final : public Surface
    {
    public:
        BackingSurface(
            vk::Device device,
            vk::PhysicalDevice physicalDevice,
            vk::UniqueSurfaceKHR&& underlying,
            vk::Sampler sampler,
            vk::Queue graphicsQueue,
            vk::Queue presentQueue,
            QueueFamilyIndices queueFamilyIndices,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            Logging::Logger& logger);
        ~BackingSurface();

        void Reinitialize(QueueFamilyIndices queueFamilyIndices);

        void StageRender(const ImageRender& imageRender) override;
        void StageRender(const LineRender& lineRender) override;
        void StageRender(const RegionRender& regionRender) override;
        void StageRender(const TextRender& textRender) override;

        void DrawFrame(const Spatial::Point2D& mapPosition, const Color& backgroundColor) override;

        void OnMaterialDestroying(const Arca::Index<Asset::Material>& material) override;

        void WaitForIdle() const;

        [[nodiscard]] Spatial::Size2D Size() const override;

        [[nodiscard]] vk::SurfaceKHR Underlying() const;
    private:
        vk::Device device;
        vk::PhysicalDevice physicalDevice;
    private:
        vk::Sampler sampler;

        std::unique_ptr<MasterRenderer> renderer;
    private:
        vk::UniqueSurfaceKHR underlying;
        vk::UniqueSwapchainKHR swapchain;
        vk::Extent2D swapchainExtent;
        std::vector<vk::UniqueImageView> imageViews;
        std::vector<vk::ImageView> nonUniqueImageViews;
    private:
        struct SwapchainDetails
        {
            vk::UniqueSwapchainKHR swapchain;
            vk::Format imageFormat;
            vk::Extent2D extent;
        };

        void Initialize(QueueFamilyIndices queueFamilyIndices);
    private:
        [[nodiscard]] static SwapchainDetails CreateSwapchain(
            QueueFamilyIndices queueFamilyIndices,
            SwapchainSupportDetails support,
            vk::Device device,
            vk::SurfaceKHR surface,
            const vk::UniqueSwapchainKHR& swapchain);

        [[nodiscard]] static vk::SurfaceFormatKHR ChooseSwapChainFormat(
            const std::vector<vk::SurfaceFormatKHR>& surfaceFormats);
        [[nodiscard]] static vk::PresentModeKHR ChooseSwapChainPresentMode(
            const std::vector<vk::PresentModeKHR>& surfacePresentModes);
    };
}