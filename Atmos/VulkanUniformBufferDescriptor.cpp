#include "VulkanUniformBufferDescriptor.h"

namespace Atmos::Render::Vulkan
{
    UniformBufferDescriptor::UniformBufferDescriptor(
        vk::Buffer buffer, uint32_t offset, uint32_t range, uint32_t binding)
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