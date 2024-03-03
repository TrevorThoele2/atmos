#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    [[nodiscard]] uint32_t FindSuitableMemoryType(
        uint32_t typeFilter, vk::MemoryPropertyFlags flags, vk::PhysicalDeviceMemoryProperties memoryProperties);
}