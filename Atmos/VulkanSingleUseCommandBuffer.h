#pragma once

#include "VulkanIncludes.h"
#include <functional>

namespace Atmos::Render::Vulkan
{
    void SingleUseCommandBuffer(
        vk::Device device,
        vk::CommandPool commandPool,
        vk::Queue queue,
        const std::function<void(vk::CommandBuffer)>& execute);
}