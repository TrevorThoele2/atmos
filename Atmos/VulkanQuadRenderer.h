#pragma once

#include "VulkanRendererBase.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include "VulkanMemoryPool.h"

#include "RenderImage.h"
#include "RenderText.h"

#include "Logger.h"

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
            vk::Device device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent);

        void StageRender(const RenderImage& imageRender);
        void StageRender(const RenderText& textRender);

        [[nodiscard]] std::unique_ptr<Raster> Start(
            vk::CommandBuffer drawCommandBuffer,
            const UniversalDataBuffer& universalDataBuffer) override;
        
        void MaterialDestroying(Arca::Index<Asset::Material> material);

        [[nodiscard]] size_t RenderCount() const override;
    private:
        std::vector<RenderImage> stagedImageRenders;
        std::vector<RenderText> stagedTextRenders;
    private:
        MemoryPool memoryPool;

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

        static constexpr int maxQuadCount = 10000;

        using Index = uint16_t;
        using Indices = std::array<Index, 6>;
        static inline const Indices indices = { 0, 1, 2, 2, 1, 3 };
        static inline constexpr Index indexIncrement = 4;

        StagedBuffer vertexBuffer;
        static constexpr int vertexStride = maxQuadCount * 4;

        StagedBuffer indexBuffer;
        static constexpr int indexStride = maxQuadCount * 6;
    private:
        using MappedConduits = MappedConduits<Asset::Material>;

        using DescriptorSetKey = QuadRendererDescriptorSetKey;

        class Raster final : public Vulkan::Raster
        {
        public:
            Raster(QuadRenderer& renderer);

            [[nodiscard]] std::vector<Pass> NextPasses() override;

            [[nodiscard]] bool IsDone() const override;
            [[nodiscard]] ObjectLayeringKey NextLayer() const override;
        private:
            using ObjectLayering = ObjectLayering<const CombinedImageSamplerDescriptor*, Quad>;
            using Layer = ObjectLayering::Layer;

            ObjectLayering layers;
            ObjectLayering::iterator currentLayer = {};
            std::unordered_map<DescriptorSetKey, vk::DescriptorSet> descriptorSets = {};

            std::uint32_t totalQuadCount = 0;
        private:
            QuadRenderer* renderer;
        private:
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer& layer);
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup);
            [[nodiscard]] Command WriteData(const std::vector<Quad>& quads, std::uint32_t startQuadCount);
            [[nodiscard]] Command Draw(std::uint32_t startQuadCount, std::uint32_t quadCount, Conduit& conduit, vk::DescriptorSet descriptorSet);
        private:
            friend QuadRenderer;
        };

        std::unordered_set<DescriptorSetKey> descriptorSetKeys;
        void AddToRaster(const RenderImage& imageRender, Raster& raster);
        void AddToRaster(const RenderText& textRender, Raster& raster);

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
        vk::Device device;
    };
}