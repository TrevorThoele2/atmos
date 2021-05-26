#pragma once

#include "VulkanRendererBase.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanCombinedImageSamplerDescriptor.h"

#include "ImageRender.h"
#include "TextRender.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    struct QuadRendererDescriptorSetKey
    {
        const CombinedImageSamplerDescriptor* descriptor;
        QuadRendererDescriptorSetKey(const CombinedImageSamplerDescriptor& descriptor);
        QuadRendererDescriptorSetKey(const QuadRendererDescriptorSetKey& arg) = default;
        bool operator==(const QuadRendererDescriptorSetKey& arg) const;
    };
}

namespace std
{
    template<>
    struct hash<Atmos::Render::Vulkan::QuadRendererDescriptorSetKey>
    {
        using argument_type = Atmos::Render::Vulkan::QuadRendererDescriptorSetKey;
        using result_type = std::size_t;

        result_type operator()(const argument_type& arg) const noexcept
        {
            const auto first(std::hash<const Atmos::Render::Vulkan::CombinedImageSamplerDescriptor*>()(arg.descriptor));
            return first;
        }
    };
}

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
            vk::Extent2D swapchainExtent);

        void StageRender(const ImageRender& imageRender);
        void StageRender(const TextRender& textRender);

        [[nodiscard]] std::unique_ptr<Raster> Start(
            vk::CommandBuffer commandBuffer,
            vk::CommandPool commandPool,
            const UniversalDataBuffer& universalDataBuffer) override;
        
        void MaterialDestroying(Arca::Index<Asset::Material> material);

        [[nodiscard]] size_t RenderCount() const override;
    private:
        std::vector<ImageRender> stagedImageRenders;
        std::vector<TextRender> stagedTextRenders;
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
        using MappedConduits = MappedConduits<Asset::Material>;

        using DescriptorSetKey = QuadRendererDescriptorSetKey;

        class Raster final : public Vulkan::Raster
        {
        public:
            Raster(
                vk::CommandBuffer commandBuffer,
                vk::CommandPool commandPool,
                QuadRenderer& renderer);

            void DrawNextLayer() override;

            [[nodiscard]] bool IsDone() const override;
            [[nodiscard]] ObjectLayeringKey NextLayer() const override;
        private:
            using ObjectLayering = ObjectLayering<const CombinedImageSamplerDescriptor*, Quad>;
            using Layer = ObjectLayering::Layer;

            vk::CommandBuffer commandBuffer;
            vk::CommandPool commandPool;

            ObjectLayering layers;
            ObjectLayering::iterator currentLayer = {};
            std::unordered_map<DescriptorSetKey, vk::DescriptorSet> setupDescriptorSets = {};

            std::uint32_t quadCount = 0;
        private:
            QuadRenderer* renderer;
        private:
            void Draw(Layer& layer);
            void WriteToBuffers(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup);
            void WriteToBuffers(const std::vector<Quad>& quads);
        private:
            friend QuadRenderer;
        };

        std::unordered_set<DescriptorSetKey> descriptorSetKeys;
        void AddToRaster(const ImageRender& imageRender, Raster& raster);
        void AddToRaster(const TextRender& textRender, Raster& raster);

        void AddToRaster(
            int space,
            Spatial::Point3D::Value z,
            Arca::RelicID materialID,
            CombinedImageSamplerDescriptor& descriptor,
            std::array<Vertex, 4> vertices,
            Raster& raster);
    private:
        [[nodiscard]] static std::array<Vertex, 4> ToVertices(
            Color color,
            Spatial::Point2D position,
            Spatial::Size2D size,
            Spatial::Angle2D rotation,
            Spatial::AxisAlignedBox2D texture);

        [[nodiscard]] static Spatial::AxisAlignedBox2D ToTextureSlice(
            const Spatial::AxisAlignedBox2D& inputSlice, const Spatial::Size2D& resourceSize);
    private:
        DescriptorSetPool descriptorSetPool;
        MappedConduits mappedConduits;
    private:
        vk::Queue graphicsQueue;

        std::shared_ptr<vk::Device> device;
    };
}