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

    void WaitAndReset(vk::Device device, vk::Fence fence, Logging::Logger& logger)
    {
        const auto onError = [&]()
        {
            logger.Log("Could not wait for Vulkan fences.");
        };

        if (IsError(device.waitForFences(fence, VK_TRUE, UINT64_MAX)))
            onError();

        if (IsError(device.resetFences(1, &fence)))
            onError();
    }
}