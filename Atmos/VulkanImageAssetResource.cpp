#include "VulkanImageAssetResource.h"

namespace Atmos::Asset::Resource::Vulkan
{
    Image::Image(
        vk::Image image,
        vk::DeviceMemory memory,
        vk::ImageView imageView,
        Render::Vulkan::CombinedImageSamplerDescriptor descriptor)
        :
        image(image),
        memory(memory),
        imageView(imageView),
        descriptor(std::move(descriptor))
    {}
}