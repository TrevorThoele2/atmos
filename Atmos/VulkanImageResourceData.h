#pragma once

#include "Size2D.h"
#include "VulkanIncludes.h"
#include "VulkanCombinedImageSamplerDescriptor.h"
#include "VulkanStoredResource.h"

namespace Atmos::Render::Resource::Vulkan
{
    class ImageData final
    {
    public:
        Spatial::Size2D size;
        vk::DeviceMemory memory;
        vk::ImageView imageView;
        vk::Image image;
        Render::Vulkan::CombinedImageSamplerDescriptor descriptor;
        Render::Vulkan::StoredResource* storedResource;
    public:
        ImageData(
            Spatial::Size2D size,
            vk::Image image,
            vk::DeviceMemory memory,
            vk::ImageView imageView,
            Render::Vulkan::CombinedImageSamplerDescriptor descriptor,
            Render::Vulkan::StoredResource& storedResource);
    };
}