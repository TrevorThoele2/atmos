#include "VulkanKeyedDescriptorSetGroup.h"

namespace Atmos::Render::Vulkan
{
    KeyedDescriptorSetGroup<void>::Set::Set(std::uint32_t imageIndex, vk::DescriptorSet descriptorSet) :
        imageIndex(imageIndex), descriptorSet(descriptorSet)
    {}

    KeyedDescriptorSetGroup<void>::KeyedDescriptorSetGroup(
        const std::vector<Definition>& definitions, uint32_t swapchainImageCount, vk::Device device)
        :
        setGroup(definitions, device), swapchainImageCount(swapchainImageCount)
    {}

    void KeyedDescriptorSetGroup<void>::Start()
    {
        SetupDiscriminatedDescriptorSet(setupDiscrimination);
    }

    auto KeyedDescriptorSetGroup<void>::SetFor(std::uint32_t imageIndex) ->Set*
    {
        auto found = std::find_if(
            sets.begin(),
            sets.end(),
            [imageIndex](const Set& set)
            {
                return imageIndex == set.imageIndex;
            });
        return found == sets.end()
            ? nullptr
            : &*found;
    }
}