#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    [[nodiscard]] vk::UniqueImageView CreateImageView(
        vk::Image image, vk::Device device, vk::Format imageFormat);
}