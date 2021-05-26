#pragma once

#include "ImageAssetResource.h"
#include "VulkanIncludes.h"
#include "VulkanCombinedImageSamplerDescriptor.h"

namespace Atmos::Asset::Resource::Vulkan
{
    class Image final : public Resource::Image
    {
    public:
        Spatial::Size2D size;
        vk::Image image;
        vk::DeviceMemory memory;
        vk::ImageView imageView;
        Render::Vulkan::CombinedImageSamplerDescriptor descriptor;
    public:
        Image(
            Spatial::Size2D size,
            vk::Image image,
            vk::DeviceMemory memory,
            vk::ImageView imageView,
            Render::Vulkan::CombinedImageSamplerDescriptor descriptor);

        [[nodiscard]] Spatial::Size2D Size() const override;
    };
}