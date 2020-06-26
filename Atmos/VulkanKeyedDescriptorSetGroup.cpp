#include "VulkanKeyedDescriptorSetGroup.h"

namespace Atmos::Render::Vulkan
{
    KeyedDescriptorSetGroup<void>::KeyedSet::KeyedSet(uint32_t imageIndex, vk::DescriptorSet descriptorSet) :
        imageIndex(imageIndex), descriptorSet(descriptorSet)
    {}

    KeyedDescriptorSetGroup<void>::KeyedDescriptorSetGroup(
        const std::vector<Definition>& definitions, vk::Device device)
        :
        setGroup(definitions, device)
    {}

    void KeyedDescriptorSetGroup<void>::Initialize(uint32_t swapchainImageCount)
    {
        this->swapchainImageCount = swapchainImageCount;
    }

    bool KeyedDescriptorSetGroup<void>::UpdateSets()
    {
        const auto iterateSources = [this](auto onEach)
        {
            for (uint32_t i = 0; i < swapchainImageCount; ++i)
            {
                const auto descriptorSet = setGroup.At(i);
                onEach(descriptorSet, i);
            }
        };

        const auto requiredSetSize = swapchainImageCount;
        if (keyedSets.size() < requiredSetSize)
        {
            keyedSets.clear();
            keyedSets.reserve(requiredSetSize);
            setGroup.Reserve(requiredSetSize);

            iterateSources([this](vk::DescriptorSet descriptorSet, uint32_t swapchainImageIndex)
                {
                    keyedSets.emplace_back(swapchainImageIndex, descriptorSet);
                });

            return true;
        }
        else
            return false;
    }

    void KeyedDescriptorSetGroup<void>::Clear()
    {
        setGroup.Reset();
        keyedSets.clear();
    }

    auto KeyedDescriptorSetGroup<void>::KeyedSetFor(uint32_t imageIndex) ->KeyedSet*
    {
        const auto found = std::find_if(
            keyedSets.begin(),
            keyedSets.end(),
            [imageIndex](const KeyedSet& set)
            {
                return imageIndex == set.imageIndex;
            });
        return found == keyedSets.end()
            ? nullptr
            : &*found;
    }

    vk::DescriptorSetLayout KeyedDescriptorSetGroup<void>::Layout() const
    {
        return setGroup.DescriptorSetLayout();
    }

    auto KeyedDescriptorSetGroup<void>::begin() -> iterator
    {
        return keyedSets.begin();
    }

    auto KeyedDescriptorSetGroup<void>::begin() const -> const_iterator
    {
        return keyedSets.begin();
    }

    auto KeyedDescriptorSetGroup<void>::end() -> iterator
    {
        return keyedSets.end();
    }

    auto KeyedDescriptorSetGroup<void>::end() const -> const_iterator
    {
        return keyedSets.end();
    }
}