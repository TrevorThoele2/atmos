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
    class QuadRenderer final : public RendererInterface
    {
    public:
        QuadRenderer(
            std::shared_ptr<vk::Device> device,
            uint32_t graphicsQueueIndex,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties);

        void Initialize(uint32_t swapchainImageCount, vk::RenderPass renderPass, vk::Extent2D extent);

        void StageRender(const ImageRender& imageRender);

        void Start(Arca::Reliquary& reliquary, vk::CommandBuffer commandBuffer);
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
            alignas(16) glm::vec4 color;
            alignas(8) glm::vec2 position;
            alignas(8) glm::vec2 texture;
        };

        struct Quad
        {
            using Vertices = std::array<Vertex, 4>;
            Vertices vertices;
            Quad(const Vertices& vertices);
        };

        static const int stride = 10000;

        using Index = uint16_t;
        using Indices = std::array<Index, 6>;
        static inline const Indices indices = { 0, 1, 2, 2, 1, 3 };
        static inline const Index indexIncrement = 4;

        StagedBuffer vertexBuffer;
        static const int vertexStride = stride * 4;

        StagedBuffer indexBuffer;
        static const int indexStride = stride * 6;

        struct Context
        {
            struct Group
            {
                std::unordered_map<const Asset::Image*, std::vector<Quad>> quads;
                std::vector<Quad>& ListFor(const Asset::Image& image);
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
            const std::vector<Quad>& quads,
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