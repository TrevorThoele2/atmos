#pragma once

namespace Atmos::Render::Vulkan
{
    struct QueueFamilyIndices
    {
        uint32_t graphicsFamily;
        uint32_t presentFamily;
    };
}