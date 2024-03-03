#pragma once

#include "VulkanRendererBase.h"
#include "VulkanTextured.h"
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
        
        [[nodiscard]] std::unique_ptr<Raster> Start(
            const AllRenders& allRenders,
            vk::CommandBuffer drawCommandBuffer,
            const UniversalDataBuffer& universalDataBuffer) override;

        void MaterialDestroying(Arca::Index<Asset::Material> material);
    private:
        struct StagedTextRender
        {
            std::vector<std::vector<Glyph>> lines;
            Spatial::Size2D totalSize;
            Spatial::Size2D atlasImageSize;
            Spatial::Size2D atlasCellSize;
            vk::ImageView atlasImageView;

            Asset::Resource::Font* fontResource;
            Spatial::AxisAlignedBox2D viewSlice;

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
        
        [[nodiscard]] std::optional<StagedTextRender> ToStagedRender(const RenderText& render);
    private:
        MemoryPool memoryPool;

        static constexpr int maxElementCount = 10000;

        StagedBuffer vertexBuffer;
        static constexpr int vertexStride = maxElementCount * 4;

        StagedBuffer indexBuffer;
        static constexpr int indexStride = maxElementCount * 6;
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
            using ObjectLayering = ObjectLayering<const CombinedImageSamplerDescriptor*, Textured>;
            using Layer = ObjectLayering::Layer;

            ObjectLayering layers;
            ObjectLayering::iterator currentLayer = {};
            std::unordered_map<DescriptorSetKey, vk::DescriptorSet> descriptorSets = {};

            std::uint32_t totalVertexCount = 0;
            std::uint32_t totalIndexCount = 0;
        private:
            TextRenderer* renderer;
        private:
            [[nodiscard]] std::vector<Pass> NextPasses(
                const Layer& layer);
            [[nodiscard]] std::vector<Pass> NextPasses(
                const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup);
            [[nodiscard]] Command WriteData(
                const std::vector<Textured>& elements, std::uint32_t startVertexCount, std::uint32_t startIndexCount);
            [[nodiscard]] Command Draw(
                std::uint32_t vertexCount, std::uint32_t indexCount, std::uint32_t startIndexCount, Conduit& conduit, vk::DescriptorSet descriptorSet);
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
            std::vector<Textured> elements,
            Raster& raster);
    private:
        [[nodiscard]] static std::vector<Textured> ToElements(const StagedTextRender& render);
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