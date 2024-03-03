#pragma once

#include "VulkanRendererInterface.h"
#include "VulkanRendererCore.h"

#include "VulkanUniformBufferDescriptor.h"
#include "VulkanStagedBuffer.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include "VulkanCommandBufferGroup.h"

#include "ImageRender.h"
#include "Position3D.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class RegionRenderer final : public RendererInterface
    {
    public:
        RegionRenderer(
            std::shared_ptr<vk::Device> device,
            uint32_t graphicsQueueIndex,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties);

        void Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent);

        void StageRender(const ImageRender& imageRender);

        void Start(const std::vector<const Asset::Material*>& materials, vk::CommandBuffer commandBuffer);
        void DrawNextLayer(uint32_t currentImage, glm::vec2 cameraSize) override;
        void End();
        [[nodiscard]] bool IsDone() const override;

        [[nodiscard]] Position3D::Value NextLayer() const override;
        [[nodiscard]] size_t LayerCount() const;
    private:
        std::optional<UniformBufferDescriptor> uniformBufferDescriptor;
    private:
        struct Vertex
        {
            alignas(8) glm::vec2 position;
        };

        struct Region
        {
            using Vertices = std::vector<Vertex>;
            Vertices vertices;
            Region(const Vertices& vertices);
        };

        static const int stride = 5000;

        StagedBuffer vertexBuffer;
        static const int vertexStride = stride * 4;

        struct Context
        {
            struct Group
            {
                std::vector<Region> regions;
            };

            std::unordered_map<const Asset::Material*, Group> groups;
            CombinedImageSamplerDescriptor descriptor;

            explicit Context(const CombinedImageSamplerDescriptor& descriptor) :
                descriptor(descriptor)
            {}

            Group& GroupFor(const Asset::Material& material);
        };

        void WriteToBuffers(
            const Context::Group& group,
            const Asset::Material* materialAsset,
            vk::CommandBuffer commandBuffer,
            std::uint32_t currentImage,
            glm::vec2 cameraSize);
        void WriteToBuffers(
            const std::vector<Region>& quads,
            const Asset::Image* imageAsset,
            Pipeline& pipeline,
            vk::CommandBuffer commandBuffer,
            std::uint32_t currentImage);
    private:
        using Core = RendererCore<const Asset::Image*, Context>;
        Core core;
    private:
        vk::Queue graphicsQueue;
        CommandBufferGroup commandBuffers;

        std::shared_ptr<vk::Device> device;
    };
}