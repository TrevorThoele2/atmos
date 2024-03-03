#pragma once

#include "VulkanBuffer.h"
#include "VulkanUniformBufferDescriptor.h"

namespace Atmos::Render::Vulkan
{
    struct ConduitExecutionContext
    {
        Buffer buffer;
        UniformBufferDescriptor descriptor;

        ConduitExecutionContext(
            vk::Device device,
            vk::PhysicalDeviceMemoryProperties memoryProperties);
    };
}