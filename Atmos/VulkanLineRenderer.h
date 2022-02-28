#pragma once

#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanMemoryPool.h"
#include "VulkanRendererPass.h"
#include "VulkanUniversalDataBuffer.h"

#include "DrawLine.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class LineRenderer final
    {
    public:
        LineRenderer(
            vk::Device device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent);

        void Start();
        [[nodiscard]] std::optional<RendererPass> Draw(
            const std::vector<Raster::DrawLine>& draws, const UniversalDataBuffer& universalDataBuffer);
    private:
        MemoryPool memoryPool;

        struct Vertex
        {
            alignas(16) glm::vec4 color;
            alignas(8) glm::vec2 position;
        };

        struct Line
        {
            using Vertices = std::vector<Vertex>;
            Vertices vertices = {};
        };

        static constexpr vk::DeviceSize maxPointCount = 5000;

        StagedBuffer vertexBuffer;
        static constexpr vk::DeviceSize vertexStride = maxPointCount;

        [[nodiscard]] Command WriteData(
            const std::vector<Line>& lines, std::uint32_t startVertexCount);
        [[nodiscard]] Command Draw(
            std::uint32_t vertexCount, std::uint32_t startVertexCount, Conduit& conduit, LineWidth lineWidth, vk::DescriptorSet descriptorSet);
    private:
        size_t totalVertexCount = 0;

        std::vector<DescriptorSetPool> descriptorSetPools;
        std::vector<DescriptorSetPool>::iterator currentDescriptorSetPool;
        MappedConduits mappedConduits;

        [[nodiscard]] static std::vector<DescriptorSetPool> CreateDescriptorSetPools(vk::Device device);
        [[nodiscard]] DescriptorSetPool& NextDescriptorSetPool();
    private:
        vk::Queue graphicsQueue;
        vk::Device device;
        uint32_t swapchainImageCount = 0;
    };
}
