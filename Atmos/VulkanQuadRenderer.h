#pragma once

#include "VulkanRendererBase.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"

#include "ImageRender.h"
#include "Point3D.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class QuadRenderer final : public RendererBase
    {
    public:
        QuadRenderer(
            std::shared_ptr<vk::Device> device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            uint32_t swapchainImageCount,
            vk::Extent2D swapchainExtent,
            const std::vector<const Asset::Material*>& materials);

        void StageRender(const ImageRender& imageRender);

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
    private:
        using ObjectLayering = ObjectLayering<const Asset::Image*, Quad>;
        using Layer = ObjectLayering::Layer;
        ObjectLayering layers;
    private:
        DescriptorSetPool descriptorSetPool;
        std::set<const Asset::Image*> stagedImageAssets;
    private:
        MappedConduits mappedConduits;
    private:
        struct SetupDescriptorSet
        {
            const Asset::Image* image;
            uint32_t swapchainImage;
            vk::DescriptorSet value;
            SetupDescriptorSet(const Asset::Image* image, uint32_t swapchainImage, vk::DescriptorSet value) :
                image(image), swapchainImage(swapchainImage), value(value)
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

            std::uint32_t quadCount = 0;
        };

        std::optional<DrawContext> drawContext;

        void Draw(Layer& layer);
        void WriteToBuffers(
            const Layer::MaterialGroup& materialGroup,
            const Asset::Material& materialAsset);
        void WriteToBuffers(
            Conduit& conduit,
            const std::vector<Quad>& quads,
            const Asset::Image* imageAsset);
    private:
        vk::Queue graphicsQueue;

        std::shared_ptr<vk::Device> device;

        uint32_t swapchainImageCount = 0;
    };
}