#pragma once

#include <optional>
#include "VulkanBuffer.h"

#include <Chroma/TypeIdentity.h>

namespace Atmos::Render::Vulkan
{
    class StagedBuffer
    {
    public:
        Buffer source;
        Buffer destination;

        StagedBuffer(
            vk::DeviceSize size,
            vk::Device device,
            vk::PhysicalDeviceMemoryProperties memoryProperties);
        StagedBuffer(
            vk::DeviceSize size,
            vk::Device device,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            vk::BufferUsageFlags destinationUsage);

        void PushSourceBytes(Bytes bytes, vk::DeviceSize offset);
        void PushSourceBytes(const void* bytes, vk::DeviceSize offset, size_t size);
        template<class T>
        void PushSourceBytes(const T& object, vk::DeviceSize offset);
        void CopyFromSourceToDestination(vk::DeviceSize size, vk::CommandPool commandPool, vk::Queue queue);
        void CopyFromSourceToDestination(vk::CommandPool commandPool, vk::Queue queue);
    private:
        struct Internal {};

        StagedBuffer(
            Chroma::TypeIdentity<Internal>,
            vk::DeviceSize size,
            vk::Device device,
            vk::PhysicalDeviceMemoryProperties memoryProperties,
            std::optional<vk::BufferUsageFlags> destinationUsage);
    };

    template<class T>
    void StagedBuffer::PushSourceBytes(const T& object, vk::DeviceSize offset)
    {
        source.PushBytes(object, offset);
    }
}