#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    struct QueueFamilyIndices
    {
        std::uint32_t graphicsFamily;
        std::uint32_t presentFamily;
    };
}