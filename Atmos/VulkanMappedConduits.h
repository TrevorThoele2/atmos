#pragma once

#include <unordered_map>
#include "VulkanConduit.h"
#include "MaterialAsset.h"

namespace Atmos::Render::Vulkan
{
    class MappedConduits
    {
    public:
        using Group = std::vector<Conduit>;
    public:
        MappedConduits(
            std::shared_ptr<vk::Device> device,
            VertexInput vertexInput,
            vk::PrimitiveTopology primitiveTopology,
            vk::RenderPass renderPass,
            vk::Extent2D swapchainExtent,
            const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts);

        void Add(Arca::Index<Asset::Material> material);
        void Remove(Arca::Index<Asset::Material> material);

        [[nodiscard]] Group* For(Arca::RelicID id);

        [[nodiscard]] vk::PipelineLayout PipelineLayout() const;
    private:
        std::unordered_map<Arca::RelicID, Group> groups;
    private:
        std::shared_ptr<vk::Device> device;

        vk::UniquePipelineLayout layout;
        VertexInput vertexInput;
        vk::PrimitiveTopology primitiveTopology;

        vk::RenderPass renderPass;
        vk::Extent2D swapchainExtent;
    private:
        [[nodiscard]] Group CreateGroup(const Asset::Material& material) const;
    };
}