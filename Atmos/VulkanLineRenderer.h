#pragma once

#include "VulkanRendererBase.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanMemoryPool.h"

#include "RenderLine.h"
#include "LineWidth.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class LineRenderer final : public RendererBase
    {
    public:
        LineRenderer(
            vk::Device device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent);

        void StageRender(const RenderLine& lineRender);
        
        [[nodiscard]] std::unique_ptr<Raster> Start(
            vk::CommandBuffer drawCommandBuffer,
            const UniversalDataBuffer& universalDataBuffer) override;
        
        void MaterialDestroying(Arca::Index<Asset::Material> material);

        [[nodiscard]] size_t RenderCount() const override;
    private:
        std::vector<RenderLine> stagedLineRenders;
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
            Vertices vertices;
            explicit Line(const std::vector<Vertex>& points);
        };

        static constexpr int maxPointCount = 5000;

        StagedBuffer vertexBuffer;
        static constexpr int vertexStride = maxPointCount;
    private:
        using MappedConduits = MappedConduits<Asset::Material>;

        class Raster final : public Vulkan::Raster
        {
        public:
            Raster(LineRenderer& renderer);
            
            [[nodiscard]] std::vector<Pass> NextPasses() override;

            [[nodiscard]] bool IsDone() const override;
            [[nodiscard]] ObjectLayeringKey NextLayer() const override;
        private:
            using ObjectLayering = ObjectLayering<LineWidth, Line>;
            using Layer = ObjectLayering::Layer;
            
            ObjectLayering layers;
            ObjectLayering::iterator currentLayer = {};
            vk::DescriptorSet setupDescriptorSet = {};

            std::uint32_t totalVertexCount = 0;
        private:
            LineRenderer* renderer;
        private:
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer& layer);
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup);
            [[nodiscard]] Command WriteData(const std::vector<Line>& lines, std::uint32_t startVertexCount);
            [[nodiscard]] Command Draw(std::uint32_t startVertexCount, std::uint32_t vertexCount, Conduit& conduit, LineWidth lineWidth);
        private:
            friend LineRenderer;
        };

        void AddToRaster(const RenderLine& lineRender, Raster& raster);
    private:
        DescriptorSetPool descriptorSetPool;
        MappedConduits mappedConduits;
    private:
        vk::Queue graphicsQueue;
        vk::Device device;
        uint32_t swapchainImageCount = 0;
    };
}
