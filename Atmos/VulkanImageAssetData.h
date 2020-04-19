#pragma once

#include "ImageAssetData.h"
#include "VulkanIncludes.h"
#include "VulkanCombinedImageSamplerDescriptor.h"

namespace Atmos::Render::Vulkan
{
    class ImageAssetDataImplementation final : public Asset::ImageAssetData
    {
    public:
        vk::UniqueImage image;
        vk::UniqueDeviceMemory memory;
        vk::UniqueImageView imageView;
        CombinedImageSamplerDescriptor descriptor;
    public:
        ImageAssetDataImplementation(
            vk::UniqueImage image,
            vk::UniqueDeviceMemory memory,
            vk::UniqueImageView imageView,
            CombinedImageSamplerDescriptor descriptor,
            Dimension width,
            Dimension height);
    };
}