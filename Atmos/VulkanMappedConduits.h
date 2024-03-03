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
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            VertexInput vertexInput,
            vk::PrimitiveTopology primitiveTopology,
            vk::RenderPass renderPass,
            uint32_t swapchainImageCount,
            vk::Extent2D swapchainExtent,
            const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts);

        void Add(const Asset::Material& material);
        void Remove(const Asset::Material& material);

        [[nodiscard]] Group* For(const Asset::Material& material);

        [[nodiscard]] vk::PipelineLayout PipelineLayout() const;
    private:
        std::unordered_map<const Asset::Material*, Group> groups;
    private:
        std::shared_ptr<vk::Device> device;

        vk::UniquePipelineLayout layout;
        VertexInput vertexInput;
        vk::PrimitiveTopology primitiveTopology;
        vk::PhysicalDeviceMemoryProperties memoryProperties;

        vk::RenderPass renderPass;
        uint32_t swapchainImageCount;
        vk::Extent2D swapchainExtent;
    private:
        [[nodiscard]] Group CreateGroup(const Asset::Material& material) const;
    };
}