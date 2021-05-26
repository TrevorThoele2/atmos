#include "VulkanImageAssetResource.h"

namespace Atmos::Asset::Resource::Vulkan
{
    Image::Image(
        Spatial::Size2D size,
        vk::Image image,
        vk::DeviceMemory memory,
        vk::ImageView imageView,
        Render::Vulkan::CombinedImageSamplerDescriptor descriptor)
        :
        size(size),
        image(image),
        memory(memory),
        imageView(imageView),
        descriptor(descriptor)
    {}

    Spatial::Size2D Image::Size() const
    {
        return size;
    }
}