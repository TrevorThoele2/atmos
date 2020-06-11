#include "VulkanUniformBufferDescriptor.h"

namespace Atmos::Render::Vulkan
{
    UniformBufferDescriptor::UniformBufferDescriptor(
        vk::Buffer buffer, std::uint32_t offset, std::uint32_t range, std::uint32_t binding)
        :
        binding(binding), bufferInfo(buffer, offset, range)
    {}

    void UniformBufferDescriptor::Update(vk::DescriptorSet descriptorSet, vk::Device device) const
    {
        const vk::WriteDescriptorSet write(
            descriptorSet,
            binding,
            0,
            1,
            vk::DescriptorType::eUniformBuffer,
            nullptr,
            &bufferInfo,
            nullptr);
        device.updateDescriptorSets(1, &write, 0, nullptr);
    }
}