#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#pragma warning(push)
#pragma warning(disable: 26812 26495 28251)
#include <vulkan/vulkan.hpp>
#pragma warning(pop)

#include "Buffer.h"

namespace Atmos::Render::Vulkan
{
    using Bytes = Buffer;
}