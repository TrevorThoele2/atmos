#include "VulkanMappedConduits.h"

#include "VulkanInvalidConduit.h"

namespace Atmos::Render::Vulkan
{
    MappedConduits::MappedConduits(
        vk::Device device,
        VertexInput vertexInput,
        vk::PrimitiveTopology primitiveTopology,
        vk::RenderPass renderPass,
        vk::Extent2D swapchainExtent,
        std::vector<vk::DynamicState> dynamicStates,
        const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts)
        :
        device(device),
        vertexInput(vertexInput),
        primitiveTopology(primitiveTopology),
        renderPass(renderPass),
        swapchainExtent(swapchainExtent),
        dynamicStates(dynamicStates)
    {
        const vk::PipelineLayoutCreateInfo layoutCreateInfo({}, descriptorSetLayouts.size(), descriptorSetLayouts.data());
        layout = device.createPipelineLayoutUnique(layoutCreateInfo);
    }
    
    Conduit& MappedConduits::For(const Shaders& shaders)
    {
        auto found = conduits.find(shaders);
        if (found == conduits.end())
            found = conduits.emplace(shaders, Conduit(shaders)).first;

        return found->second;
    }

    vk::PipelineLayout MappedConduits::PipelineLayout() const
    {
        return layout.get();
    }

    Conduit MappedConduits::Conduit(const Shaders& shaders) const
    {
        return
        {
            shaders.vertex,
            shaders.fragment,
            device,
            layout.get(),
            renderPass,
            vertexInput,
            swapchainExtent,
            primitiveTopology,
            dynamicStates
        };
    }
}