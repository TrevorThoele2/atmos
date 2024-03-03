#include "VulkanDescriptorSetPool.h"

#include <unordered_map>

#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    DescriptorSetPool::DescriptorSetPool(
        const std::vector<Definition>& definitions, std::shared_ptr<vk::Device> device)
        :
        definitions(definitions), device(device)
    {
        if (definitions.empty())
            throw GraphicsError("DescriptorSetGroup requires definitions.");

        std::vector<vk::DescriptorSetLayoutBinding> bindings;
        for (auto& definition : definitions)
        {
            vk::DescriptorSetLayoutBinding binding;
            binding.descriptorCount = definition.count;
            binding.descriptorType = definition.type;
            binding.binding = definition.binding;
            binding.stageFlags = definition.stageFlags;
            bindings.push_back(binding);
        }
        const vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo({}, bindings.size(), bindings.data());
        descriptorSetLayout = device->createDescriptorSetLayoutUnique(descriptorSetLayoutCreateInfo);
    }

    void DescriptorSetPool::Reserve(uint32_t count)
    {
        if (count <= availableDescriptorSets.size())
            return;

        AttemptAllocate(count);
    }

    void DescriptorSetPool::Reset()
    {
        availableDescriptorSets = allDescriptorSets;
    }

    vk::DescriptorSet DescriptorSetPool::Next()
    {
        if (availableDescriptorSets.empty())
            AttemptAllocate(allDescriptorSets.size() + 1);

        const auto returnValue = availableDescriptorSets[availableDescriptorSets.size() - 1];
        availableDescriptorSets.pop_back();
        return returnValue;
    }

    size_t DescriptorSetPool::Size() const
    {
        return allDescriptorSets.size();
    }

    vk::DescriptorSetLayout DescriptorSetPool::DescriptorSetLayout() const
    {
        return descriptorSetLayout.get();
    }

    void DescriptorSetPool::AttemptAllocate(uint32_t count)
    {
        std::vector<vk::DescriptorPoolSize> poolSizes;

        for(auto& definition : definitions)
        {
            vk::DescriptorPoolSize poolSize;
            poolSize.type = definition.type;
            poolSize.descriptorCount = definition.count;
            poolSizes.push_back(poolSize);
        }

        const vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo({}, count, poolSizes.size(), poolSizes.data());
        descriptorPool = device->createDescriptorPoolUnique(descriptorPoolCreateInfo);

        const std::vector<vk::DescriptorSetLayout> layouts(count, *descriptorSetLayout);
        const vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo(*descriptorPool, count, layouts.data());
        allDescriptorSets = device->allocateDescriptorSets(descriptorSetAllocateInfo);
        availableDescriptorSets = allDescriptorSets;
    }
}