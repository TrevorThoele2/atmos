#include "VulkanCreateImageView.h"

namespace Atmos::Render::Vulkan
{
    vk::UniqueImageView CreateImageView(
        vk::Image image, vk::Device device, vk::Format imageFormat)
    {
        const vk::ImageViewCreateInfo createInfo(
            {},
            image,
            vk::ImageViewType::e2D,
            imageFormat,
            vk::ComponentMapping(),
            vk::ImageSubresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });

        return device.createImageViewUnique(createInfo);
    }
}