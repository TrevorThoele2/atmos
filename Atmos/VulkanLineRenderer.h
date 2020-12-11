#pragma once

#include "VulkanRendererBase.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"

#include "LineRender.h"
#include "LineWidth.h"
#include "Point3D.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class LineRenderer final : public RendererBase
    {
    public:
        LineRenderer(
            std::shared_ptr<vk::Device> device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent,
            const Arca::Batch<Asset::LineMaterial>& materials);

        void StageRender(const LineRender& lineRender);

        [[nodiscard]] std::unique_ptr<Raster> Start(
            vk::CommandBuffer commandBuffer,
            vk::CommandPool commandPool,
            const UniversalDataBuffer& universalDataBuffer) override;

        void MaterialCreated(Arca::Index<Asset::LineMaterial> material);
        void MaterialDestroying(Arca::Index<Asset::LineMaterial> material);

        [[nodiscard]] size_t RenderCount() const override;
    private:
        std::vector<LineRender> stagedLineRenders;
    private:
        struct Vertex
        {
            alignas(16) glm::vec4 color;
            alignas(8) glm::vec2 position;
        };

        struct Line
        {
            using Vertices = std::vector<Vertex>;
            Vertices vertices;
            explicit Line(const std::vector<Vertex>& points);
        };

        static const int stride = 5000;

        StagedBuffer vertexBuffer;
        static const int vertexStride = stride;
    private:
        using MappedConduits = MappedConduits<Asset::LineMaterial>;

        class Raster final : public Vulkan::Raster
        {
        public:
            Raster(
                vk::CommandBuffer commandBuffer,
                vk::CommandPool commandPool,
                LineRenderer& renderer);

            void DrawNextLayer() override;

            [[nodiscard]] bool IsDone() const override;
            [[nodiscard]] Spatial::Point3D::Value NextLayer() const override;
        private:
            using ObjectLayering = ObjectLayering<LineWidth, Line>;
            using Layer = ObjectLayering::Layer;

            vk::CommandBuffer commandBuffer;
            vk::CommandPool commandPool;

            ObjectLayering layers;
            ObjectLayering::iterator currentLayer = {};
            vk::DescriptorSet setupDescriptorSet = {};

            std::uint32_t vertexCount = 0;
        private:
            LineRenderer* renderer;
        private:
            void Draw(Layer& layer);
            void WriteToBuffers(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup);
            void WriteToBuffers(const std::vector<Line>& lines);
        private:
            friend LineRenderer;
        };

        void AddToRaster(const LineRender& lineRender, Raster& raster);
    private:
        DescriptorSetPool descriptorSetPool;
        MappedConduits mappedConduits;
    private:
        vk::Queue graphicsQueue;

        std::shared_ptr<vk::Device> device;

        uint32_t swapchainImageCount = 0;
    };
}