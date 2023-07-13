#pragma once

#include "VulkanQuad.h"
#include "VulkanDescriptorSetPool.h"
#include "VulkanMappedConduits.h"
#include "VulkanStagedBuffer.h"
#include "VulkanMemoryPool.h"
#include "VulkanUniversalDataBuffer.h"
#include "VulkanRendererPass.h"
#include "VulkanGlyphAtlas.h"

#undef DrawText

#include "DrawImage.h"
#include "DrawText.h"

namespace Atmos::Render::Vulkan
{
    class QuadRenderer final
    {
    public:
        QuadRenderer(
            vk::Device device,
            vk::Queue graphicsQueue,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent,
            GlyphAtlas& glyphAtlas);

        void Start();
        [[nodiscard]] std::optional<RendererPass> Draw(
            const std::vector<Raster::DrawImage>& draws, const UniversalDataBuffer& universalDataBuffer);
        [[nodiscard]] std::optional<RendererPass> Draw(
            const std::vector<Raster::DrawText>& draws, const UniversalDataBuffer& universalDataBuffer);
    private:
        MemoryPool memoryPool;

        static constexpr vk::DeviceSize maxElementCount = 10000;

        StagedBuffer vertexBuffer;
        static constexpr vk::DeviceSize vertexStride = maxElementCount * verticesPerQuad;

        StagedBuffer indexBuffer;
        static constexpr vk::DeviceSize indexStride = maxElementCount * indicesPerQuad;

        [[nodiscard]] Command WriteData(
            const std::vector<Quad>& elements,
            std::uint32_t startElementCount);
        [[nodiscard]] Command Draw(
            std::uint32_t vertexCount,
            std::uint32_t indexCount,
            std::uint32_t startIndexCount,
            Conduit& conduit,
            vk::DescriptorSet descriptorSet);

        [[nodiscard]] std::vector<Quad> ToQuads(const Raster::DrawText& draw);
    private:
        size_t totalElements = 0;

        std::vector<DescriptorSetPool> descriptorSetPools;
        std::vector<DescriptorSetPool>::iterator currentDescriptorSetPool;
        MappedConduits mappedConduits;

        GlyphAtlas* glyphAtlas;

        [[nodiscard]] static std::vector<DescriptorSetPool> CreateDescriptorSetPools(vk::Device device);
        [[nodiscard]] DescriptorSetPool& NextDescriptorSetPool();
    private:
        vk::Queue graphicsQueue;
        vk::Device device;
    };
}
