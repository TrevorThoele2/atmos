#pragma once

#include <unordered_map>
#include "VulkanConduit.h"

#include "Shaders.h"

namespace Atmos::Render::Vulkan
{
    class MappedConduits
    {
    public:
        MappedConduits(
            vk::Device device,
            VertexInput vertexInput,
            vk::PrimitiveTopology primitiveTopology,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent,
            std::vector<vk::DynamicState> dynamicStates,
            const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts);
        
        [[nodiscard]] Conduit& For(const Shaders& shaders);
        [[nodiscard]] vk::PipelineLayout PipelineLayout() const;
    private:
        std::unordered_map<Shaders, Conduit> conduits;
    private:
        vk::Device device;

        vk::UniquePipelineLayout layout;
        VertexInput vertexInput;
        vk::PrimitiveTopology primitiveTopology;

        vk::RenderPass renderPass;
        vk::Extent2D swapchainExtent;

        std::vector<vk::DynamicState> dynamicStates;
    private:
        [[nodiscard]] Conduit Conduit(const Shaders& shaders) const;
    };
}