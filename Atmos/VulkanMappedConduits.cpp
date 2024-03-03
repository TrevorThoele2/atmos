#include "VulkanMappedConduits.h"

namespace Atmos::Render::Vulkan
{
    MappedConduits::MappedConduits(
        std::shared_ptr<vk::Device> device,
        VertexInput vertexInput,
        vk::PrimitiveTopology primitiveTopology,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts)
        :
        device(device),
        vertexInput(vertexInput),
        primitiveTopology(primitiveTopology),
        renderPass(renderPass),
        swapchainExtent(swapchainExtent)
    {
        const vk::PipelineLayoutCreateInfo layoutCreateInfo({}, descriptorSetLayouts.size(), descriptorSetLayouts.data());
        layout = device->createPipelineLayoutUnique(layoutCreateInfo);
    }

    void MappedConduits::Add(const Asset::Material& material)
    {
        groups.emplace(&material, CreateGroup(material));
    }

    void MappedConduits::Remove(const Asset::Material& material)
    {
        groups.erase(&material);
    }

    auto MappedConduits::For(const Asset::Material& material) -> Group*
    {
        auto found = groups.find(&material);
        if (found == groups.end())
            return nullptr;

        return &found->second;
    }

    vk::PipelineLayout MappedConduits::PipelineLayout() const
    {
        return layout.get();
    }

    auto MappedConduits::CreateGroup(const Asset::Material& material) const -> Group
    {
        const auto& passes = material.Passes();

        Group group;
        group.reserve(passes.size());
        for (auto& pass : passes)
        {
            group.push_back(Conduit(
                pass.VertexShader(),
                pass.FragmentShader(),
                *device,
                layout.get(),
                renderPass,
                vertexInput,
                swapchainExtent,
                primitiveTopology,
                {}));
        }
        return group;
    }
}