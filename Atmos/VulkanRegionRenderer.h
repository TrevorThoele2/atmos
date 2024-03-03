#pragma once

#include "VulkanRendererBase.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"

#include "RegionRender.h"
#include "Point3D.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class RegionRenderer final : public RendererBase
    {
    public:
        RegionRenderer(
            std::shared_ptr<vk::Device> device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent,
            const Arca::Batch<Asset::RegionMaterial>& materials);

        void StageRender(const RegionRender& regionRender);

        [[nodiscard]] std::unique_ptr<Raster> Start(
            vk::CommandBuffer commandBuffer,
            vk::CommandPool commandPool,
            const UniversalDataBuffer& universalDataBuffer) override;

        void MaterialCreated(Arca::Index<Asset::RegionMaterial> material);
        void MaterialDestroying(Arca::Index<Asset::RegionMaterial> material);

        [[nodiscard]] size_t RenderCount() const override;
    private:
        std::vector<RegionRender> stagedRegionRenders;
    private:
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

        static const int stride = 5000;

        StagedBuffer vertexBuffer;
        static const int vertexStride = stride;

        StagedBuffer indexBuffer;
        static const int indexStride = stride * 2;
    private:
        using MappedConduits = MappedConduits<Asset::RegionMaterial>;

        class Raster final : public Vulkan::Raster
        {
        public:
            Raster(
                vk::CommandBuffer commandBuffer,
                vk::CommandPool commandPool,
                RegionRenderer& renderer);

            void DrawNextLayer() override;

            [[nodiscard]] bool IsDone() const override;
            [[nodiscard]] Spatial::Point3D::Value NextLayer() const override;
        private:
            using ObjectLayering = ObjectLayering<void, Region>;
            using Layer = ObjectLayering::Layer;

            vk::CommandBuffer commandBuffer;
            vk::CommandPool commandPool;

            ObjectLayering layers;
            ObjectLayering::iterator currentLayer = {};
            vk::DescriptorSet setupDescriptorSet = {};

            std::uint32_t vertexCount = 0;
            std::uint32_t indexCount = 0;
            Index maxIndex = 0;
        private:
            RegionRenderer* renderer;
        private:
            void Draw(Layer& layer);
            void WriteToBuffers(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup);
            void WriteToBuffers(const std::vector<Region>& regions);
        private:
            friend RegionRenderer;
        };

        void AddToRaster(const RegionRender& regionRender, Raster& raster);
    private:
        DescriptorSetPool descriptorSetPool;
        MappedConduits mappedConduits;
    private:
        vk::Queue graphicsQueue;

        std::shared_ptr<vk::Device> device;

        uint32_t swapchainImageCount = 0;
    };
}