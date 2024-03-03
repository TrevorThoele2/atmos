#include "VulkanDescriptorSetGroup.h"

#include <unordered_map>

#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    DescriptorSetGroup::DescriptorSetGroup(const std::vector<Definition>& definitions, vk::Device device) :
        definitions(definitions), device(device)
    {
        if (definitions.empty())
            throw GraphicsError("DescriptorSetGroup requires definitions.");
    }

    vk::DescriptorSet DescriptorSetGroup::operator[](uint32_t index) const
    {
        return At(index);
    }

    void DescriptorSetGroup::Reserve(uint32_t count)
    {
        if (count <= availableDescriptorSets.size())
            return;

        AttemptAllocate(count);
    }

    void DescriptorSetGroup::Reset()
    {
        availableDescriptorSets = allDescriptorSets;
    }

    vk::DescriptorSet DescriptorSetGroup::At(uint32_t index) const
    {
        return allDescriptorSets[index];
    }

    vk::DescriptorSet DescriptorSetGroup::Next()
    {
        if (availableDescriptorSets.empty())
            AttemptAllocate(allDescriptorSets.size() + 1);

        const auto returnValue = availableDescriptorSets[availableDescriptorSets.size() - 1];
        availableDescriptorSets.pop_back();
        return returnValue;
    }

    size_t DescriptorSetGroup::Size() const
    {
        return allDescriptorSets.size();
    }

    vk::DescriptorSetLayout DescriptorSetGroup::DescriptorSetLayout() const
    {
        return descriptorSetLayout.get();
    }

    void DescriptorSetGroup::AttemptAllocate(uint32_t count)
    {
        if (descriptorPool)
        {
            descriptorPool.reset();
            descriptorSetLayout.reset();
            availableDescriptorSets.clear();
            allDescriptorSets.clear();
        }

        std::vector<vk::DescriptorPoolSize> poolSizes;
        std::vector<vk::DescriptorSetLayoutBinding> bindings;

        for(auto& definition : definitions)
        {
            vk::DescriptorPoolSize poolSize;
            poolSize.type = definition.type;
            poolSize.descriptorCount = definition.count;
            poolSizes.push_back(poolSize);

            vk::DescriptorSetLayoutBinding binding;
            binding.descriptorCount = definition.count;
            binding.descriptorType = definition.type;
            binding.binding = definition.binding;
            binding.stageFlags = definition.stageFlags;
            bindings.push_back(binding);
        }

        const vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo({}, count, poolSizes.size(), poolSizes.data());
        descriptorPool = device.createDescriptorPoolUnique(descriptorPoolCreateInfo);

        const vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo({}, bindings.size(), bindings.data());
        descriptorSetLayout = device.createDescriptorSetLayoutUnique(descriptorSetLayoutCreateInfo);

        const std::vector<vk::DescriptorSetLayout> layouts(count, *descriptorSetLayout);
        const vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo(*descriptorPool, count, layouts.data());
        allDescriptorSets = device.allocateDescriptorSets(descriptorSetAllocateInfo);
        availableDescriptorSets = allDescriptorSets;
    }
}