#pragma once

#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanMemoryPool.h"
#include "VulkanRendererPass.h"
#include "VulkanUniversalDataBuffer.h"

#include "DrawRegion.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class RegionRenderer final
    {
    public:
        RegionRenderer(
            vk::Device device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent);

        void Start();
        [[nodiscard]] std::optional<RendererPass> Draw(
            const std::vector<Raster::DrawRegion>& draws, const UniversalDataBuffer& universalDataBuffer);
    private:
        MemoryPool memoryPool;

        struct Vertex
        {
            alignas(8) glm::vec2 position;
        };

        using Index = uint16_t;

        struct Region
        {
            using Vertices = std::vector<Vertex>;
            Vertices vertices = {};

            using Indices = std::vector<Index>;
            Indices indices = {};
        };

        static constexpr vk::DeviceSize maxVertexCount = 5000;

        StagedBuffer vertexBuffer;
        static constexpr vk::DeviceSize vertexStride = maxVertexCount;

        StagedBuffer indexBuffer;
        static constexpr vk::DeviceSize indexStride = maxVertexCount * 2;

        [[nodiscard]] Command WriteData(
            const std::vector<Region>& regions, std::uint32_t startIndexCount);
        [[nodiscard]] Command Draw(
            std::uint32_t startIndexCount, std::uint32_t indexCount, Conduit& conduit, vk::DescriptorSet descriptorSet);
    private:
        size_t totalVertices = 0;
        size_t totalIndices = 0;
        Index maxIndex = 0;

        std::vector<DescriptorSetPool> descriptorSetPools;
        std::vector<DescriptorSetPool>::iterator currentDescriptorSetPool;
        MappedConduits mappedConduits;

        [[nodiscard]] static std::vector<DescriptorSetPool> CreateDescriptorSetPools(vk::Device device);
        [[nodiscard]] DescriptorSetPool& NextDescriptorSetPool();
    private:
        vk::Queue graphicsQueue;
        vk::Device device;
    };
}