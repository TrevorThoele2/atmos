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
            uint32_t swapchainImageCount,
            vk::Extent2D swapchainExtent,
            const std::vector<const Asset::Material*>& materials);

        void StageRender(const LineRender& lineRender);

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
        using ObjectLayering = ObjectLayering<LineWidth, Line>;
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
        };

        std::optional<DrawContext> drawContext;

        void Draw(Layer& layer);
        void WriteToBuffers(
            const Layer::MaterialGroup& group,
            const Asset::Material& materialAsset);
        void WriteToBuffers(
            Conduit& conduit,
            const std::vector<Line>& lines,
            LineWidth width);
    private:
        vk::Queue graphicsQueue;

        std::shared_ptr<vk::Device> device;

        uint32_t swapchainImageCount = 0;
    };
}