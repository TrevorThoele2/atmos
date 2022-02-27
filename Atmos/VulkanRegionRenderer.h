#pragma once

#include "VulkanRendererBase.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanMemoryPool.h"

#include "RenderRegion.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class RegionRenderer final : public RendererBase
    {
    public:
        RegionRenderer(
            vk::Device device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent);
        
        [[nodiscard]] std::unique_ptr<Raster> Start(
            const AllRenders& allRenders,
            vk::CommandBuffer drawCommandBuffer,
            const UniversalDataBuffer& universalDataBuffer) override;
        
        void MaterialDestroying(const Asset::Material& material);
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
            Vertices vertices;

            using Indices = std::vector<Index>;
            Indices indices;

            Region(const Vertices& vertices, const Indices& indices);
        };

        static constexpr vk::DeviceSize maxVertexCount = 5000;

        StagedBuffer vertexBuffer;
        static constexpr vk::DeviceSize vertexStride = maxVertexCount;

        StagedBuffer indexBuffer;
        static constexpr vk::DeviceSize indexStride = maxVertexCount * 2;
    private:
        class Raster final : public Vulkan::Raster
        {
        public:
            explicit Raster(RegionRenderer& renderer);
            
            [[nodiscard]] std::vector<Pass> NextPasses() override;

            [[nodiscard]] bool IsDone() const override;
            [[nodiscard]] ObjectLayeringKey NextLayer() const override;
        private:
            using ObjectLayering = ObjectLayering<void, Region>;
            using Layer = ObjectLayering::Layer;
            
            ObjectLayering layers;
            ObjectLayering::iterator currentLayer = {};
            vk::DescriptorSet setupDescriptorSet = {};

            std::uint32_t totalVertexCount = 0;
            std::uint32_t totalIndexCount = 0;
            Index maxIndex = 0;
        private:
            RegionRenderer* renderer;
        private:
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer& layer);
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup);
            [[nodiscard]] Command WriteData(const std::vector<Region>& regions, std::uint32_t startIndexCount);
            [[nodiscard]] Command Draw(std::uint32_t startIndexCount, std::uint32_t indexCount, Conduit& conduit);
        private:
            friend RegionRenderer;
        };

        void AddToRaster(const RenderRegion& regionRender, Raster& raster);
    private:
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