#include "VulkanDescriptorSetPool.h"

#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    DescriptorSetPool::DescriptorSetPool(const std::vector<Definition>& definitions, vk::Device device) :
        definitions(definitions), descriptorSetLayout(CreateDescriptorSetLayout(definitions, device)), device(device)
    {}

    std::vector<vk::DescriptorSet> DescriptorSetPool::Retrieve(uint32_t count)
    {
        if (count > descriptorSets.size())
            Allocate(count);

        std::vector<vk::DescriptorSet> retrievedDescriptorSets;
        for (uint32_t i = 0; i < count; ++i)
            retrievedDescriptorSets.push_back(descriptorSets[i]);

        return retrievedDescriptorSets;
    }

    size_t DescriptorSetPool::Size() const
    {
        return descriptorSets.size();
    }

    vk::DescriptorSetLayout DescriptorSetPool::DescriptorSetLayout() const
    {
        return descriptorSetLayout.get();
    }

    vk::UniqueDescriptorSetLayout DescriptorSetPool::CreateDescriptorSetLayout(
        const std::vector<Definition>& definitions, vk::Device device)
    {
        if (definitions.empty())
            throw GraphicsError("DescriptorSetPool requires definitions.");

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
        const auto createInfo = vk::DescriptorSetLayoutCreateInfo({}, bindings.size(), bindings.data());

        return device.createDescriptorSetLayoutUnique(createInfo);
    }

    void DescriptorSetPool::Allocate(uint32_t count)
    {
        std::vector<vk::DescriptorPoolSize> poolSizes;

        for(const auto& definition : definitions)
        {
            vk::DescriptorPoolSize poolSize;
            poolSize.type = definition.type;
            poolSize.descriptorCount = definition.count * count;
            poolSizes.push_back(poolSize);
        }

        const vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo({}, count, poolSizes.size(), poolSizes.data());
        descriptorPool = device.createDescriptorPoolUnique(descriptorPoolCreateInfo);

        const std::vector layouts(count, *descriptorSetLayout);
        const vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo(*descriptorPool, count, layouts.data());
        descriptorSets = device.allocateDescriptorSets(descriptorSetAllocateInfo);
    }
}