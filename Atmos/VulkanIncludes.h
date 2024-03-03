#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include "WindowsUndef.h"

#include "Buffer.h"

namespace Atmos::Render::Vulkan
{
    using Bytes = Buffer;
}