#include "VulkanSynchronization.h"

#include "VulkanResults.h"

namespace Atmos::Render::Vulkan
{
    vk::UniqueSemaphore CreateSemaphore(vk::Device device, vk::SemaphoreCreateFlags flags)
    {
        const vk::SemaphoreCreateInfo createInfo(flags);
        return device.createSemaphoreUnique(createInfo);
    }

    vk::UniqueFence CreateFence(vk::Device device, vk::FenceCreateFlags flags)
    {
        const vk::FenceCreateInfo createInfo(flags);
        return device.createFenceUnique(createInfo);
    }

    vk::Result WaitAndReset(vk::Device device, vk::Fence fence)
    {
        const auto waitResult = device.waitForFences(fence, VK_TRUE, UINT64_MAX);
        if (IsError(waitResult))
            return waitResult;

        return device.resetFences(1, &fence);
    }
}