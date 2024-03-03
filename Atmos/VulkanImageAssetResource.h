#pragma once

#include "ImageAssetResource.h"
#include "VulkanIncludes.h"
#include "VulkanImage.h"
#include "VulkanCombinedImageSamplerDescriptor.h"

namespace Atmos::Asset::Resource::Vulkan
{
    class Image final : public Resource::Image
    {
    public:
        ImageSize size;
        vk::Image image;
        vk::DeviceMemory memory;
        vk::ImageView imageView;
        Render::Vulkan::CombinedImageSamplerDescriptor descriptor;
    public:
        Image(
            ImageSize size,
            vk::Image image,
            vk::DeviceMemory memory,
            vk::ImageView imageView,
            Render::Vulkan::CombinedImageSamplerDescriptor descriptor);

        [[nodiscard]] ImageSize Size() const override;
    };
}