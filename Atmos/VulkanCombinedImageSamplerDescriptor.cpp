#include "VulkanCombinedImageSamplerDescriptor.h"

namespace Atmos::Render::Vulkan
{
    CombinedImageSamplerDescriptor::CombinedImageSamplerDescriptor(
        vk::ImageView imageView, vk::Sampler sampler, vk::ImageLayout imageLayout, uint32_t binding)
        :
        binding(binding), imageInfo(sampler, imageView, imageLayout)
    {}

    void CombinedImageSamplerDescriptor::Update(vk::DescriptorSet descriptorSet, vk::Device device) const
    {
        const vk::WriteDescriptorSet write(
            descriptorSet,
            binding,
            0,
            1,
            vk::DescriptorType::eCombinedImageSampler,
            &imageInfo,
            nullptr,
            nullptr);
        device.updateDescriptorSets(1, &write, 0, nullptr);
    }
}