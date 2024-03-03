#include "VulkanUniformBufferDescriptor.h"

namespace Atmos::Render::Vulkan
{
    UniformBufferDescriptor::UniformBufferDescriptor(
        vk::Buffer buffer, std::uint32_t offset, std::uint32_t range, std::uint32_t binding)
        :
        binding(binding), bufferInfo(buffer, offset, range)
    {}

    vk::WriteDescriptorSet UniformBufferDescriptor::Write(vk::DescriptorSet descriptorSet)
    {
        return vk::WriteDescriptorSet(
            descriptorSet,
            binding,
            0,
            1,
            vk::DescriptorType::eUniformBuffer,
            nullptr,
            &bufferInfo,
            nullptr);
    }
}