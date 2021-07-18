#pragma once

#include "ImageAssetResource.h"
#include "VulkanImageResourceData.h"

namespace Atmos::Asset::Resource::Vulkan
{
    class Image final : public Resource::Image
    {
    public:
        Render::Resource::Vulkan::ImageData imageData;
    public:
        Image(const Render::Resource::Vulkan::ImageData& imageData);

        [[nodiscard]] Spatial::Size2D Size() const override;
    };
}