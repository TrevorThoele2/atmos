#pragma once

#include "VulkanBuffer.h"
#include "VulkanUniversalData.h"

namespace Atmos::Render::Vulkan
{
    class UniversalDataBuffer
    {
    public:
        UniversalDataBuffer(
            uint32_t binding,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::Device device);

        void PushBytes(UniversalData universalData);
        void Update(vk::DescriptorSet descriptorSet) const;
    private:
        Buffer buffer;
        uint32_t binding;
        vk::DescriptorBufferInfo bufferInfo;

        vk::Device device;
    };
}