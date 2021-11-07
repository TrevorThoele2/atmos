#pragma once

#include "VulkanStoredResource.h"
#include "VulkanIncludes.h"
#include "VulkanMemory.h"

namespace Atmos::Render::Vulkan
{
    class StoredImageResource final : public StoredResource
    {
    public:
        UniqueMemory memory;
        vk::UniqueImageView imageView;
        vk::UniqueImage image;

        StoredImageResource(UniqueMemory&& memory, vk::UniqueImageView&& imageView, vk::UniqueImage&& image);
    };
}