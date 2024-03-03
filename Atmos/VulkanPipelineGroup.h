#pragma once

#include "VulkanIncludes.h"
#include "VulkanPipeline.h"

namespace Atmos::Render::Vulkan
{
    class PipelineGroup
    {
    private:
        using Pipelines = std::vector<Pipeline>;
        Pipelines pipelines;
    public:
        using iterator = Pipelines::iterator;
        using const_iterator = Pipelines::const_iterator;
    public:
        PipelineGroup(
            std::shared_ptr<vk::Device> device,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            VertexInput vertexInput,
            vk::PrimitiveTopology primitiveTopology,
            Asset::MaterialType materialAssetType);
    public:
        void Recreate(
            const std::vector<const Asset::Material*>& materials,
            vk::DescriptorSetLayout descriptorSetLayout,
            uint32_t swapchainImageCount,
            vk::RenderPass renderPass,
            vk::Extent2D extent);
        Pipeline* Find(const Asset::Material& material);

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;
    private:
        std::shared_ptr<vk::Device> device;

        VertexInput vertexInput;
        vk::PrimitiveTopology primitiveTopology;
        vk::PhysicalDeviceMemoryProperties memoryProperties;
        Asset::MaterialType materialAssetType;

        Pipeline Create(
            const Asset::Material* material,
            vk::DescriptorSetLayout descriptorSetLayout,
            uint32_t swapchainImageCount,
            vk::RenderPass renderPass,
            vk::Extent2D extent);
    };
}