#include "VulkanStoredImageResource.h"

namespace Atmos::Render::Vulkan
{
    StoredImageResource::StoredImageResource(
        vk::UniqueDeviceMemory&& memory, vk::UniqueImageView&& imageView, vk::UniqueImage&& image)
        :
        memory(std::move(memory)), imageView(std::move(imageView)), image(std::move(image))
    {}
}