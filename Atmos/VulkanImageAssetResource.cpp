#include "VulkanImageAssetResource.h"

namespace Atmos::Asset::Resource::Vulkan
{
    Image::Image(const Render::Vulkan::ImageData& imageData) : imageData(imageData)
    {}

    Spatial::Size2D Image::Size() const
    {
        return imageData.size;
    }
}