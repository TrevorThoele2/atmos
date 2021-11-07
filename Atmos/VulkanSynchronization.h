#pragma once

#include "VulkanIncludes.h"

#undef CreateSemaphore

namespace Atmos::Render::Vulkan
{
    [[nodiscard]] vk::UniqueSemaphore CreateSemaphore(vk::Device device, vk::SemaphoreCreateFlags flags);
    [[nodiscard]] vk::UniqueFence CreateFence(vk::Device device, vk::FenceCreateFlags flags);

    vk::Result WaitAndReset(vk::Device device, vk::Fence fence);
}