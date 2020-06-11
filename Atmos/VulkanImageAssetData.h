#pragma once

#include "ImageAssetData.h"
#include "VulkanIncludes.h"
#include "VulkanImage.h"
#include "VulkanCombinedImageSamplerDescriptor.h"

namespace Atmos::Render::Vulkan
{
    class ImageAssetDataImplementation final : public Asset::ImageData
    {
    public:
        Image image;
        vk::UniqueImageView imageView;
        CombinedImageSamplerDescriptor descriptor;
    public:
        ImageAssetDataImplementation(
            Image&& image,
            vk::UniqueImageView&& imageView,
            CombinedImageSamplerDescriptor descriptor);
    };
}