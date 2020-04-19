#include "VulkanCombinedImageSamplerDescriptor.h"

namespace Atmos::Render::Vulkan
{
    CombinedImageSamplerDescriptor::CombinedImageSamplerDescriptor(
        vk::ImageView imageView, vk::Sampler sampler, vk::ImageLayout imageLayout, std::uint32_t binding)
        :
        binding(binding), imageInfo(sampler, imageView, imageLayout)
    {}

    vk::WriteDescriptorSet CombinedImageSamplerDescriptor::Write(vk::DescriptorSet descriptorSet)
    {
        return vk::WriteDescriptorSet(
            descriptorSet,
            binding,
            0,
            1,
            vk::DescriptorType::eCombinedImageSampler,
            &imageInfo,
            nullptr,
            nullptr);
    }
}