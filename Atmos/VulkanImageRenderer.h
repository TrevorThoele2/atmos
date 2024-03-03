#pragma once

#include "VulkanRendererBase.h"
#include "VulkanQuad.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include "VulkanMemoryPool.h"

#include "RenderImage.h"

#include "Logger.h"

namespace Atmos::Render::Vulkan
{
    struct ImageRendererDescriptorSetKey
    {
        const CombinedImageSamplerDescriptor* descriptor;
        ImageRendererDescriptorSetKey(const CombinedImageSamplerDescriptor& descriptor);
        ImageRendererDescriptorSetKey(const ImageRendererDescriptorSetKey& arg) = default;
        bool operator==(const ImageRendererDescriptorSetKey& arg) const;
    };
}

namespace std
{
    template<>
    struct hash<Atmos::Render::Vulkan::ImageRendererDescriptorSetKey>
    {
        using argument_type = Atmos::Render::Vulkan::ImageRendererDescriptorSetKey;
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
    class ImageRenderer final : public RendererBase
    {
    public:
        ImageRenderer(
            vk::Device device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent);

        void StageRender(const RenderImage& imageRender);

        [[nodiscard]] std::unique_ptr<Raster> Start(
            vk::CommandBuffer drawCommandBuffer,
            const UniversalDataBuffer& universalDataBuffer) override;
        
        void MaterialDestroying(Arca::Index<Asset::Material> material);

        [[nodiscard]] size_t RenderCount() const override;
    private:
        std::vector<RenderImage> stagedImageRenders;
    private:
        MemoryPool memoryPool;

        static constexpr int maxQuadCount = 10000;

        StagedBuffer vertexBuffer;
        static constexpr int vertexStride = maxQuadCount * 4;

        StagedBuffer indexBuffer;
        static constexpr int indexStride = maxQuadCount * 6;
    private:
        using MappedConduits = MappedConduits<Asset::Material>;

        using DescriptorSetKey = ImageRendererDescriptorSetKey;

        class Raster final : public Vulkan::Raster
        {
        public:
            Raster(ImageRenderer& renderer);

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
            ImageRenderer* renderer;
        private:
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer& layer);
            [[nodiscard]] std::vector<Pass> NextPasses(const Layer::MaterialGroup& materialGroup, MappedConduits::Group& conduitGroup);
            [[nodiscard]] Command WriteData(const std::vector<Quad>& quads, std::uint32_t startQuadCount);
            [[nodiscard]] Command Draw(std::uint32_t startQuadCount, std::uint32_t quadCount, Conduit& conduit, vk::DescriptorSet descriptorSet);
        private:
            friend ImageRenderer;
        };

        std::unordered_set<DescriptorSetKey> descriptorSetKeys;

        void AddToRaster(const RenderImage& imageRender, Raster& raster);
        void AddToRaster(
            int space,
            Spatial::Point3D::Value z,
            Arca::RelicID materialID,
            CombinedImageSamplerDescriptor& descriptor,
            std::array<QuadVertex, 4> vertices,
            Raster& raster);
    private:
        DescriptorSetPool descriptorSetPool;
        MappedConduits mappedConduits;
    private:
        vk::Queue graphicsQueue;
        vk::Device device;
    };
}