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
            uint32_t swapchainImageCount,
            vk::Extent2D swapchainExtent,
            const std::vector<const Asset::Material*>& materials);

        void StageRender(const RegionRender& regionRender);

        void Start(
            vk::CommandBuffer commandBuffer,
            vk::CommandPool commandPool,
            uint32_t currentSwapchainImage,
            UniversalData universalData) override;
        void DrawNextLayer() override;
        void End() override;

        void MaterialCreated(const Asset::Material& material) override;
        void MaterialDestroying(const Asset::Material& material) override;

        [[nodiscard]] bool IsDone() const override;
        [[nodiscard]] Spatial::Point3D::Value NextLayer() const override;
        [[nodiscard]] size_t LayerCount() const override;
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
        using ObjectLayering = ObjectLayering<void, Region>;
        using Layer = ObjectLayering::Layer;
        ObjectLayering layers;
    private:
        DescriptorSetPool descriptorSetPool;
    private:
        MappedConduits mappedConduits;
    private:
        struct SetupDescriptorSet
        {
            uint32_t swapchainImage;
            vk::DescriptorSet value;
            SetupDescriptorSet(uint32_t swapchainImage, vk::DescriptorSet value) :
                swapchainImage(swapchainImage), value(value)
            {}
        };

        struct DrawContext
        {
            std::vector<const Asset::Material*> materials = {};
            vk::CommandBuffer commandBuffer = {};
            vk::CommandPool commandPool = {};
            ObjectLayering::iterator currentLayer = {};
            uint32_t currentSwapchainImage = 0;
            UniversalData universalData = {};
            std::vector<SetupDescriptorSet> setupDescriptorSets = {};

            std::uint32_t vertexCount = 0;
            std::uint32_t indexCount = 0;
        };

        std::optional<DrawContext> drawContext;

        void Draw(Layer& layer);
        void WriteToBuffers(
            const Layer::MaterialGroup& group,
            const Asset::Material& materialAsset);
        void WriteToBuffers(
            Conduit& conduit,
            const std::vector<Region>& regions);
    private:
        vk::Queue graphicsQueue;

        std::shared_ptr<vk::Device> device;

        uint32_t swapchainImageCount = 0;
    };
}