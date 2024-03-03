#pragma once

#include "VulkanStoredResource.h"
#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class StoredImageResource final : public StoredResource
    {
    public:
        vk::UniqueDeviceMemory memory;
        vk::UniqueImageView imageView;
        vk::UniqueImage image;

        StoredImageResource(
            vk::UniqueDeviceMemory&& memory, vk::UniqueImageView&& imageView, vk::UniqueImage&& image);
    };
}