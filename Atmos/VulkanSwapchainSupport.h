#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    struct SwapchainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    [[nodiscard]] SwapchainSupportDetails SwapchainSupport(
        vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
}