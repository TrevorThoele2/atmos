#pragma once

#include "VulkanIncludes.h"
#include "VulkanPipeline.h"

namespace Atmos::Render::Vulkan
{
    class PipelineGroup
    {
    private:
        using Pipelines = std::unordered_map<const Asset::Material*, std::vector<Pipeline>>;
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
            const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
            uint32_t swapchainImageCount,
            vk::RenderPass renderPass,
            vk::Extent2D extent);
        std::vector<Pipeline>* Find(const Asset::Material& material);

        [[nodiscard]] vk::PipelineLayout Layout() const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;
    private:
        std::shared_ptr<vk::Device> device;

        vk::UniquePipelineLayout layout;

        VertexInput vertexInput;
        vk::PrimitiveTopology primitiveTopology;
        vk::PhysicalDeviceMemoryProperties memoryProperties;
        Asset::MaterialType materialAssetType;

        std::vector<Pipeline> CreatePipelines(
            const Asset::Material& material,
            uint32_t swapchainImageCount,
            vk::RenderPass renderPass,
            vk::Extent2D extent);
    };
}