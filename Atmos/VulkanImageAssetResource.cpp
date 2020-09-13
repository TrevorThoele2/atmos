#include "VulkanImageAssetResource.h"

namespace Atmos::Asset::Resource::Vulkan
{
    Image::Image(
        ImageSize size,
        vk::Image image,
        vk::DeviceMemory memory,
        vk::ImageView imageView,
        Render::Vulkan::CombinedImageSamplerDescriptor descriptor)
        :
        size(size),
        image(image),
        memory(memory),
        imageView(imageView),
        descriptor(std::move(descriptor))
    {}

    ImageSize Image::Size() const
    {
        return size;
    }
}