#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class CombinedImageSamplerDescriptor
    {
    public:
        CombinedImageSamplerDescriptor(
            vk::ImageView imageView, vk::Sampler sampler, vk::ImageLayout imageLayout, std::uint32_t binding);
    public:
        [[nodiscard]] vk::WriteDescriptorSet Write(vk::DescriptorSet descriptorSet);
    private:
        std::uint32_t binding;
        vk::DescriptorImageInfo imageInfo;
    };
}