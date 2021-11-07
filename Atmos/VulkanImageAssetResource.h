#pragma once

#include "ImageAssetResource.h"
#include "VulkanImage.h"

namespace Atmos::Asset::Resource::Vulkan
{
    class Image final : public Resource::Image
    {
    public:
        Render::Vulkan::ImageData imageData;
    public:
        Image(const Render::Vulkan::ImageData& imageData);

        [[nodiscard]] Spatial::Size2D Size() const override;
    };
}