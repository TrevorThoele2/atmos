#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class DescriptorSetPool
    {
    public:
        struct Definition
        {
            vk::DescriptorType type;
            uint32_t binding;
            vk::ShaderStageFlags stageFlags;
            uint32_t count;
        };
    public:
        DescriptorSetPool(const std::vector<Definition>& definitions, vk::Device device);

        [[nodiscard]] std::vector<vk::DescriptorSet> Retrieve(uint32_t count);

        [[nodiscard]] size_t Size() const;

        [[nodiscard]] vk::DescriptorSetLayout DescriptorSetLayout() const;
    private:
        std::vector<Definition> definitions;

        [[nodiscard]] static vk::UniqueDescriptorSetLayout CreateDescriptorSetLayout(
            const std::vector<Definition>& definitions, vk::Device device);
    private:
        vk::UniqueDescriptorPool descriptorPool;
        vk::UniqueDescriptorSetLayout descriptorSetLayout;
        std::vector<vk::DescriptorSet> descriptorSets;

        void Allocate(uint32_t count);
    private:
        vk::Device device;
    };
}