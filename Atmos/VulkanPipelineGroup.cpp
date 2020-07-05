#include "VulkanPipelineGroup.h"

namespace Atmos::Render::Vulkan
{
    PipelineGroup::PipelineGroup(
        std::shared_ptr<vk::Device> device,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        VertexInput vertexInput,
        vk::PrimitiveTopology primitiveTopology,
        Asset::MaterialType materialAssetType)
        :
        device(device),
        vertexInput(vertexInput),
        primitiveTopology(primitiveTopology),
        memoryProperties(memoryProperties),
        materialAssetType(materialAssetType)
    {}

    void PipelineGroup::Recreate(
        const std::vector<const Asset::Material*>& materials,
        const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts,
        uint32_t swapchainImageCount,
        vk::RenderPass renderPass,
        vk::Extent2D extent)
    {
        pipelines.clear();
        pipelines.reserve(materials.size());

        const vk::PipelineLayoutCreateInfo layoutCreateInfo({}, descriptorSetLayouts.size(), descriptorSetLayouts.data());
        layout = device->createPipelineLayoutUnique(layoutCreateInfo);

        for (auto& material : materials)
        {
            if (material->Type() != materialAssetType)
                continue;

            pipelines.emplace(
                material,
                CreatePipelines(*material, swapchainImageCount, renderPass, extent));
        }
    }

    std::vector<Pipeline>* PipelineGroup::Find(const Asset::Material& material)
    {
        const auto found = pipelines.find(&material);
        if (found == pipelines.end())
            return {};

        return &found->second;
    }

    vk::PipelineLayout PipelineGroup::Layout() const
    {
        return layout.get();
    }

    auto PipelineGroup::begin() -> iterator
    {
        return pipelines.begin();
    }

    auto PipelineGroup::begin() const -> const_iterator
    {
        return pipelines.begin();
    }

    auto PipelineGroup::end() -> iterator
    {
        return pipelines.end();
    }

    auto PipelineGroup::end() const -> const_iterator
    {
        return pipelines.end();
    }

    std::vector<Pipeline> PipelineGroup::CreatePipelines(
        const Asset::Material& material,
        uint32_t swapchainImageCount,
        vk::RenderPass renderPass,
        vk::Extent2D extent)
    {
        std::vector<Pipeline> returnValue;
        for(auto& pass : material.Passes())
        {
            if (!pass.VertexShader() || !pass.FragmentShader())
                continue;

            returnValue.push_back(Pipeline(
                &*pass.VertexShader(),
                &*pass.FragmentShader(),
                *device,
                layout.get(),
                swapchainImageCount,
                memoryProperties,
                renderPass,
                vertexInput,
                extent,
                primitiveTopology,
                {}));
        }

        return returnValue;
    }
}