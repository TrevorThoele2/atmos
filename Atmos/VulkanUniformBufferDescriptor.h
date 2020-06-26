#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class UniformBufferDescriptor
    {
    public:
        UniformBufferDescriptor(vk::Buffer buffer, uint32_t offset, uint32_t range, uint32_t binding);

        void Update(vk::DescriptorSet descriptorSet, vk::Device device) const;
    private:
        uint32_t binding;
        vk::DescriptorBufferInfo bufferInfo;
    };
}