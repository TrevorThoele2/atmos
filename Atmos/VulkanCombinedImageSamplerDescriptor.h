#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class CombinedImageSamplerDescriptor
    {
    public:
        CombinedImageSamplerDescriptor() = default;
        CombinedImageSamplerDescriptor(
            vk::ImageView imageView, vk::Sampler sampler, vk::ImageLayout imageLayout, std::uint32_t binding);
    public:
        void Update(vk::DescriptorSet descriptorSet, vk::Device device) const;
    private:
        std::uint32_t binding;
        vk::DescriptorImageInfo imageInfo;
    };
}