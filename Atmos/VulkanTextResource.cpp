#include "VulkanTextResource.h"

namespace Atmos::Render::Resource::Vulkan
{
    Text::Text(
        vk::Image image,
        vk::DeviceMemory memory,
        vk::ImageView imageView,
        Render::Vulkan::CombinedImageSamplerDescriptor descriptor,
        Spatial::Size2D size)
        :
        image(image), memory(memory), imageView(imageView), descriptor(descriptor), size(size)
    {}
}