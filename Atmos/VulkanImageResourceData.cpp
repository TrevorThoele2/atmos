#include "VulkanImageResourceData.h"

namespace Atmos::Render::Resource::Vulkan
{
    ImageData::ImageData(
        Spatial::Size2D size,
        vk::Image image,
        vk::DeviceMemory memory,
        vk::ImageView imageView,
        Render::Vulkan::CombinedImageSamplerDescriptor descriptor,
        Render::Vulkan::StoredResource& storedResource)
        :
        size(size),
        memory(memory),
        imageView(imageView),
        image(image),
        descriptor(descriptor),
        storedResource(&storedResource)
    {}
}