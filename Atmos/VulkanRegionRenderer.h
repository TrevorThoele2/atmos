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

        void StageRender(const RenderRegion& regionRender);
        
        [[nodiscard]] std::unique_ptr<Raster> Start(
            vk::CommandBuffer drawCommandBuffer,
            const UniversalDataBuffer& universalDataBuffer) override;
        
        void MaterialDestroying(Arca::Index<Asset::Material> material);

        [[nodiscard]] size_t RenderCount() const override;
    private:
        std::vector<RenderRegion> stagedRegionRenders;
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

        static constexpr int maxVertexCount = 5000;

        StagedBuffer vertexBuffer;
        static constexpr int vertexStride = maxVertexCount;

        StagedBuffer indexBuffer;
        static constexpr int indexStride = maxVertexCount * 2;
    private:
        using MappedConduits = MappedConduits<Asset::Material>;

        class Raster final : public Vulkan::Raster
        {
        public:
            Raster(RegionRenderer& renderer);
            
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
        DescriptorSetPool descriptorSetPool;
        MappedConduits mappedConduits;
    private:
        vk::Queue graphicsQueue;
        vk::Device device;
        uint32_t swapchainImageCount = 0;
    };
}