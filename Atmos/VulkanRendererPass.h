#pragma once

#include "VulkanCommand.h"

namespace Atmos::Render::Vulkan
{
    struct RendererPass
    {
        Command writeData;
        Command draw;
    };
}