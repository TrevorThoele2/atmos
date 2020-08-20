#include "VulkanUniversalDataBuffer.h"

namespace Atmos::Render::Vulkan
{
    UniversalDataBuffer::UniversalDataBuffer(
        uint32_t binding,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::Device device)
        :
        buffer(sizeof(UniversalData),
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            device,
            memoryProperties),
        binding(binding),
        bufferInfo(buffer.value.get(), 0, sizeof(UniversalData)),
        device(device)
    {}

    void UniversalDataBuffer::PushBytes(UniversalData universalData)
    {
        buffer.PushBytes(universalData, 0);
    }

    void UniversalDataBuffer::Update(vk::DescriptorSet descriptorSet) const
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