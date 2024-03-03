#include "VulkanUtilities.h"

namespace Atmos::Render::Vulkan
{
    bool IsSuccess(vk::Result result)
    {
        return result == vk::Result::eSuccess;
    }

    bool IsSuccess(VkResult result)
    {
        return result == VK_SUCCESS;
    }

    bool IsError(vk::Result result)
    {
        return result != vk::Result::eSuccess;
    }

    bool IsError(VkResult result)
    {
        return result != VK_SUCCESS;
    }
}