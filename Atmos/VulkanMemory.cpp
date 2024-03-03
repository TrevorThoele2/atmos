#include "VulkanMemory.h"

#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    uint32_t FindSuitableMemoryType(
        uint32_t typeFilter, vk::MemoryPropertyFlags flags, vk::PhysicalDeviceMemoryProperties memoryProperties)
    {
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
            if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & flags) == flags)
                return i;

        throw GraphicsError("Could not find suitable memory type.");
    }
}