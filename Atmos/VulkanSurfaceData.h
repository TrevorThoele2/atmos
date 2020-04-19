#pragma once

#include "SurfaceData.h"
#include "VulkanRenderer.h"
#include "VulkanIncludes.h"

#include "VulkanPipeline.h"
#include "VulkanLineShaders.h"
#include "VulkanQueueFamilyIndices.h"
#include "VulkanSwapchainSupport.h"

namespace Atmos::Render::Vulkan
{
    class SurfaceDataImplementation final : public SurfaceData
    {
    public:
        SurfaceDataImplementation(
            std::shared_ptr<vk::Device> device,
            vk::PhysicalDevice physicalDevice,
            vk::UniqueSurfaceKHR&& underlying,
            vk::Sampler sampler,
            LineShaders lineShaders,
            vk::Queue graphicsQueue,
            vk::Queue presentQueue,
            vk::CommandPool commandPool,
            QueueFamilyIndices queueFamilyIndices,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            Arca::Reliquary& reliquary);
        ~SurfaceDataImplementation();

        void Reinitialize(QueueFamilyIndices queueFamilyIndices);

        void StageRender(const ImageRender& imageRender) override;
        void StageRender(const LineRender& lineRender) override;

        void DrawFrame(const Color& backgroundColor) override;

        [[nodiscard]] ScreenSize Size() const override;

        [[nodiscard]] vk::SurfaceKHR Underlying() const;
    private:
        std::shared_ptr<vk::Device> device;
        vk::PhysicalDevice physicalDevice;
    private:
        vk::Sampler sampler;

        std::unique_ptr<Renderer> renderer;
    private:
        vk::UniqueSurfaceKHR underlying;
        vk::UniqueSwapchainKHR swapchain;
        vk::Extent2D swapchainExtent;
        std::vector<vk::UniqueImageView> imageViews;
        std::vector<vk::ImageView> nonUniqueImageViews;

        vk::CommandPool commandPool;
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