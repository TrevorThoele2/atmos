#pragma once

#include "VulkanIncludes.h"
#include "VulkanCommandBufferGroup.h"

#include "VulkanQuadRenderer.h"
#include "VulkanLineRenderer.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ImageRender.h"
#include "LineRender.h"
#include "ScreenSize.h"

#include "Event.h"

#undef CreateSemaphore

namespace Atmos::Render::Vulkan
{
    class Renderer final
    {
    public:
        Event<> onOutOfDate;
    public:
        Renderer(
            std::shared_ptr<vk::Device> device,
            vk::Sampler sampler,
            vk::Queue graphicsQueue,
            vk::Queue presentQueue,
            uint32_t graphicsQueueIndex,
            vk::PhysicalDeviceMemoryProperties memoryProperties);
        ~Renderer();

        void Initialize(
            vk::SwapchainKHR swapchain,
            std::vector<vk::Image> images,
            std::vector<vk::ImageView> imageViews,
            vk::Format imageFormat,
            vk::Extent2D extent);
    public:
        void StageRender(const ImageRender& imageRender);
        void StageRender(const LineRender& lineRender);

        void DrawFrame(Arca::Reliquary& reliquary, const ScreenSize& cameraSize);

        void WaitForIdle() const;
    private:
        std::shared_ptr<vk::Device> device;
    private:
        static const int maxFramesInFlight = 2;
        size_t currentFrame = 0;
        size_t previousFrame = 1;
    private:
        QuadRenderer quadRenderer;
        LineRenderer lineRenderer;

        using AllRenderers = std::vector<RendererInterface*>;
        AllRenderers allRenderers;
    private:
        void Draw(
            Arca::Reliquary& reliquary,
            std::vector<vk::CommandBuffer>& usedCommandBuffers,
            std::uint32_t currentImage,
            glm::vec2 cameraSize);
        static void ClearImage(vk::Image image, std::array<float, 4> color, vk::CommandBuffer commandBuffer);
    private:
        vk::SwapchainKHR swapchain;

        std::vector<vk::Image> images;
        std::vector<vk::ImageView> imageViews;

        vk::UniqueRenderPass renderPass;
        std::vector<vk::UniqueFramebuffer> framebuffers;
        vk::Extent2D extent;
    private:
        [[nodiscard]] static vk::UniqueRenderPass CreateRenderPass(
            vk::Device device, vk::Format swapchainImageFormat);

        [[nodiscard]] static std::vector<vk::UniqueFramebuffer> CreateFramebuffers(
            vk::Device device, const std::vector<vk::ImageView>& imageViews, vk::RenderPass renderPass, vk::Extent2D extent);
    private:
        vk::Sampler sampler;

        vk::PhysicalDeviceMemoryProperties memoryProperties;
    private:
        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        CommandBufferGroup commandBuffers;
    private:
        std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
        std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
        std::vector<vk::UniqueFence> inFlightFences;
        std::vector<vk::Fence> imagesInFlight;

        [[nodiscard]] static std::vector<vk::UniqueSemaphore> CreateSemaphores(vk::Device device, size_t count);
        [[nodiscard]] static std::vector<vk::UniqueFence> CreateFences(vk::Device device, size_t count);
    };
}