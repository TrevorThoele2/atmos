#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    bool IsSuccess(vk::Result result);
    bool IsSuccess(VkResult result);
    bool IsError(vk::Result result);
    bool IsError(VkResult result);
}