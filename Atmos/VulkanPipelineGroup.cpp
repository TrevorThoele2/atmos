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
        vk::DescriptorSetLayout descriptorSetLayout,
        uint32_t swapchainImageCount,
        vk::RenderPass renderPass,
        vk::Extent2D extent)
    {
        pipelines.clear();
        pipelines.reserve(materials.size());

        for (auto& material : materials)
        {
            if (material->Type() != materialAssetType || !material->VertexShader() || !material->FragmentShader())
                continue;

            pipelines.push_back(Create(material, descriptorSetLayout, swapchainImageCount, renderPass, extent));
        }
    }

    Pipeline* PipelineGroup::Find(const Asset::Material& material)
    {
        for (auto& pipeline : pipelines)
            if (pipeline.material == &material)
                return &pipeline;

        return nullptr;
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

    auto PipelineGroup::Create(
        const Asset::Material* material,
        vk::DescriptorSetLayout descriptorSetLayout,
        uint32_t swapchainImageCount,
        vk::RenderPass renderPass,
        vk::Extent2D extent)

        -> Pipeline
    {
        return Pipeline
        {
            *material,
            *device,
            swapchainImageCount,
            memoryProperties,
            { descriptorSetLayout },
            renderPass,
            vertexInput,
            extent,
            primitiveTopology,
            {}
        };
    }
}