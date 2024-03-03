#pragma once

#include "TextResource.h"
#include "VulkanIncludes.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include "Size2D.h"

namespace Atmos::Render::Resource::Vulkan
{
    class Text final : public Resource::Text
    {
    public:
        vk::Image image;
        vk::DeviceMemory memory;
        vk::ImageView imageView;
        Render::Vulkan::CombinedImageSamplerDescriptor descriptor;

        Spatial::Size2D size;
    public:
        Text(
            vk::Image image,
            vk::DeviceMemory memory,
            vk::ImageView imageView,
            Render::Vulkan::CombinedImageSamplerDescriptor descriptor,
            Spatial::Size2D size);
    };
}