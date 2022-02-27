#pragma once

#include "VulkanRendererBase.h"
#include "VulkanTextured.h"
#include "VulkanObjectLayering.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include "VulkanMemoryPool.h"

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
        
        [[nodiscard]] std::unique_ptr<Raster> Start(
            const AllRenders& allRenders,
            vk::CommandBuffer drawCommandBuffer,
            const UniversalDataBuffer& universalDataBuffer) override;
        
        void MaterialDestroying(const Asset::Material& material);
    private:
        MemoryPool memoryPool;

        static constexpr int maxElementCount = 10000;

        StagedBuffer vertexBuffer;
        static constexpr int vertexStride = maxElementCount * 4;

        StagedBuffer indexBuffer;
        static constexpr int indexStride = maxElementCount * 6;
    private:
        using DescriptorSetKey = ImageRendererDescriptorSetKey;

        class Raster final : public Vulkan::Raster
        {
        public:
            Raster(ImageRenderer& renderer);

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
            ImageRenderer* renderer;
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
            friend ImageRenderer;
        };

        std::unordered_set<DescriptorSetKey> descriptorSetKeys;

        void AddToRaster(const RenderImage& imageRender, Raster& raster);
        void AddToRaster(
            int space,
            Spatial::Point3D::Value z,
            const Asset::Material& material,
            const CombinedImageSamplerDescriptor& descriptor,
            Textured element,
            Raster& raster);
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