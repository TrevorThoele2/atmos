#include "VulkanConduitExecutionContext.h"
#include "VulkanUniversalData.h"

namespace Atmos::Render::Vulkan
{
    ConduitExecutionContext::ConduitExecutionContext(
        vk::Device device,
        vk::PhysicalDeviceMemoryProperties memoryProperties)
        :
        buffer(sizeof(Vulkan::UniversalData),
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            device,
            memoryProperties),
        descriptor(buffer.value.get(), 0, sizeof(UniversalData), 0)
    {}
}