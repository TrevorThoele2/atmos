#pragma once

#include "VulkanRendererBase.h"
#include "VulkanQuad.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include "VulkanMemoryPool.h"
#include "VulkanGlyphAtlas.h"

#include "RenderText.h"
#include "FontAssetResource.h"

#include "Size2D.h"

#include "Logger.h"

namespace Atmos::Render::Vulkan
{
    struct TextRendererDescriptorSetKey
    {
        const CombinedImageSamplerDescriptor* descriptor;
        TextRendererDescriptorSetKey(const CombinedImageSamplerDescriptor& descriptor);
        TextRendererDescriptorSetKey(const TextRendererDescriptorSetKey& arg) = default;
        bool operator==(const TextRendererDescriptorSetKey& arg) const;
    };
}

namespace std
{
    template<>
    struct hash<Atmos::Render::Vulkan::TextRendererDescriptorSetKey>
    {
        using argument_type = Atmos::Render::Vulkan::TextRendererDescriptorSetKey;
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
    class TextRenderer final : public RendererBase
    {
    public:
        TextRenderer(
            vk::Device device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent,
            GlyphAtlas& glyphAtlas);

        void StageRender(const RenderText& textRender);

        [[nodiscard]] std::unique_ptr<Raster> Start(
            vk::CommandBuffer drawCommandBuffer,
            const UniversalDataBuffer& universalDataBuffer) override;

        void MaterialDestroying(Arca::Index<Asset::Material> material);

        [[nodiscard]] size_t RenderCount() const override;
    private:
        struct StagedTextRender
        {
            std::vector<std::vector<Glyph>> lines;
            Spatial::Size2D totalSize;
            Spatial::Size2D atlasImageSize;
            Spatial::Size2D atlasCellSize;
            vk::ImageView atlasImageView;

            Asset::Resource::Font* fontResource;
            Spatial::AxisAlignedBox2D slice;

            Arca::Index<Asset::Material> material;

            Spatial::Point3D position;
            Spatial::Angle2D rotation;
            Spatial::Scalers2D scalers;

            Color color;

            bool bold;
            bool italics;
            float wrapWidth;

            int space;

            Resource::Surface* surface;
        };

        std::vector<StagedTextRender> stagedTextRenders;

        [[nodiscard]] std::optional<StagedTextRender> ToStagedRender(const RenderText& render);
    private:
        MemoryPool memoryPool;

        static constexpr int maxQuadCount = 10000;

        StagedBuffer vertexBuffer;
        static constexpr int vertexStride = maxQuadCount * 4;

        StagedBuffer indexBuffer;
        static constexpr int indexStride = maxQuadCount * 6;
    private:
        using MappedConduits = MappedConduits<Asset::Material>;

        using DescriptorSetKey = TextRendererDescriptorSetKey;

        class Raster final : public Vulkan::Raster
        {
        public:
            Raster(TextRenderer& renderer);

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
            TextRenderer* renderer;
        private:
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer& layer);
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup);
            [[nodiscard]] Command WriteData(const std::vector<Quad>& quads, std::uint32_t startQuadCount);
            [[nodiscard]] Command Draw(std::uint32_t startQuadCount, std::uint32_t quadCount, Conduit& conduit, vk::DescriptorSet descriptorSet);
        private:
            friend TextRenderer;
        };

        std::unordered_set<DescriptorSetKey> descriptorSetKeys;
        std::unordered_map<FontID, CombinedImageSamplerDescriptor> descriptors;

        void AddToRaster(const StagedTextRender& stagedTextRender, Raster& raster);
        void AddToRaster(
            int space,
            Spatial::Point3D::Value z,
            Arca::RelicID materialID,
            Vulkan::CombinedImageSamplerDescriptor& descriptor,
            std::vector<Quad> vertices,
            Raster& raster);
    private:
        [[nodiscard]] static std::vector<Quad> ToQuads(const StagedTextRender& render);
    private:
        vk::UniqueSampler sampler;

        GlyphAtlas* glyphAtlas;
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