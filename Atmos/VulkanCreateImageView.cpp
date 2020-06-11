#include "VulkanCreateImageView.h"

namespace Atmos::Render::Vulkan
{
    vk::UniqueImageView CreateImageView(
        vk::Image image, vk::Device device, vk::Format imageFormat, uint32_t layerOffset, uint32_t layerCount)
    {
        const vk::ImageViewCreateInfo createInfo(
            {},
            image,
            vk::ImageViewType::e2D,
            imageFormat,
            vk::ComponentMapping(),
            vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, layerOffset, layerCount));

        return device.createImageViewUnique(createInfo);
    }
}