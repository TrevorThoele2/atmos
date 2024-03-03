#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class UniformBufferDescriptor
    {
    public:
        UniformBufferDescriptor(vk::Buffer buffer, std::uint32_t offset, std::uint32_t range, std::uint32_t binding);

        [[nodiscard]] vk::WriteDescriptorSet Write(vk::DescriptorSet descriptorSet);
    private:
        std::uint32_t binding;
        vk::DescriptorBufferInfo bufferInfo;
    };
}