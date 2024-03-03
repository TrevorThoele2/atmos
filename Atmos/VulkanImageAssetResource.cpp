#include "VulkanImageAssetResource.h"

namespace Atmos::Asset::Resource::Vulkan
{
    Image::Image(const Render::Resource::Vulkan::ImageData& imageData) : imageData(imageData)
    {}

    Spatial::Size2D Image::Size() const
    {
        return imageData.size;
    }
}