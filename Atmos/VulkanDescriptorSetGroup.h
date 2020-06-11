#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class DescriptorSetGroup
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
        DescriptorSetGroup(const std::vector<Definition>& definitions, vk::Device device);

        [[nodiscard]] vk::DescriptorSet operator[](uint32_t index) const;

        void Reserve(uint32_t count);
        void Reset();

        [[nodiscard]] vk::DescriptorSet At(uint32_t index) const;
        [[nodiscard]] vk::DescriptorSet Next();

        [[nodiscard]] size_t Size() const;

        [[nodiscard]] vk::DescriptorSetLayout DescriptorSetLayout() const;
    private:
        std::vector<Definition> definitions;
    private:
        vk::UniqueDescriptorPool descriptorPool;
        vk::UniqueDescriptorSetLayout descriptorSetLayout;
        std::vector<vk::DescriptorSet> availableDescriptorSets;
        std::vector<vk::DescriptorSet> allDescriptorSets;

        void AttemptAllocate(uint32_t count);
    private:
        vk::Device device;
    };
}