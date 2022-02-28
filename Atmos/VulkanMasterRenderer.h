#pragma once

#include "VulkanIncludes.h"
#include "VulkanCommandBufferPool.h"
#include "VulkanMemoryPool.h"
#include "VulkanUniversalDataBuffer.h"

#include "VulkanQuadRenderer.h"
#include "VulkanLineRenderer.h"
#include "VulkanRegionRenderer.h"

#include "VulkanGlyphAtlas.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RasterCommand.h"
#include "Size2D.h"
#include "Point2D.h"

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
            vk::Device device,
            vk::Sampler sampler,
            vk::Queue graphicsQueue,
            vk::Queue presentQueue,
            uint32_t graphicsQueueIndex,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            GlyphAtlas& glyphAtlas,
            Logging::Logger& logger);
        ~MasterRenderer();

        void Initialize(
            vk::SwapchainKHR swapchain,
            std::vector<vk::Image> swapchainImages,
            std::vector<vk::ImageView> swapchainImageViews,
            vk::Format imageFormat,
            vk::Extent2D swapchainExtent);
    public:
        void DrawFrame(
            const std::vector<Render::Raster::Command>& commands,
            const Spatial::Size2D& screenSize,
            const Spatial::Point2D& mapPosition);

        void WaitForIdle() const;
    private:
        vk::Device device;
    private:
        size_t currentFrame = 0;
        size_t previousFrame = 1;
    private:
        struct RendererGroup
        {
            QuadRenderer quad;
            LineRenderer line;
            RegionRenderer region;

            RendererGroup(
                vk::Device device,
                vk::Queue graphicsQueue,
                vk::PhysicalDeviceMemoryProperties memoryProperties,
                vk::RenderPass renderPass,
                vk::Extent2D swapchainExtent,
                GlyphAtlas& glyphAtlas);
            RendererGroup(RendererGroup&& arg) noexcept = default;
        };
        using RendererGroups = std::list<RendererGroup>;
        RendererGroups rendererGroups;
        RendererGroups::iterator currentRendererGroup;
    private:
        std::vector<vk::CommandBuffer> usedCommandBuffers;

        void Draw(
            const std::vector<Raster::Command>& commands,
            RendererGroup& rendererGroup,
            UniversalData universalData,
            vk::Framebuffer framebuffer,
            vk::CommandBuffer commandBuffer);

        template<class DrawT>
        [[nodiscard]] std::vector<DrawT> GroupDrawCommands(
            std::vector<Raster::Command>::const_iterator& command, const std::vector<Raster::Command>& commands);
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
        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        CommandBufferPool drawCommandBuffers;

        MemoryPool memoryPool;
    private:
        UniversalDataBuffer universalDataBuffer;
    private:
        std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
        std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
        std::vector<vk::UniqueFence> inFlightFences;
        std::vector<vk::Fence> swapchainImagesInFlight;

        [[nodiscard]] static std::vector<vk::UniqueSemaphore> CreateSemaphores(vk::Device device, size_t count);
        [[nodiscard]] static std::vector<vk::UniqueFence> CreateFences(vk::Device device, size_t count);
    private:
        GlyphAtlas* glyphAtlas;
    private:
        Logging::Logger* logger;
    };

    template<class DrawT>
    std::vector<DrawT> MasterRenderer::GroupDrawCommands(
        std::vector<Raster::Command>::const_iterator& command, const std::vector<Raster::Command>& commands)
    {
        std::vector<DrawT> group;
        while (command != commands.end() && std::holds_alternative<DrawT>(*command))
        {
            group.push_back(std::get<DrawT>(*command));
            ++command;
        }
        return group;
    }
}