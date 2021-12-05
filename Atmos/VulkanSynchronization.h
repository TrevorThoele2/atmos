#pragma once

#include "VulkanIncludes.h"
#include "Logger.h"

#undef CreateSemaphore

namespace Atmos::Render::Vulkan
{
    [[nodiscard]] vk::UniqueSemaphore CreateSemaphore(vk::Device device, vk::SemaphoreCreateFlags flags);
    [[nodiscard]] vk::UniqueFence CreateFence(vk::Device device, vk::FenceCreateFlags flags);

    void WaitAndReset(vk::Device device, vk::Fence fence, Logging::Logger& logger);
}