#include "VulkanSwapchainSupport.h"

namespace Atmos::Render::Vulkan
{
    SwapchainSupportDetails SwapchainSupport(
        vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
    {
        return
        {
            physicalDevice.getSurfaceCapabilitiesKHR(surface),
            physicalDevice.getSurfaceFormatsKHR(surface),
            physicalDevice.getSurfacePresentModesKHR(surface)
        };
    }
}