#pragma once

#include <optional>
#include "VulkanBuffer.h"
#include "VulkanCommand.h"

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
            MemoryPool& pool);
        StagedBuffer(
            vk::DeviceSize size,
            vk::Device device,
            MemoryPool& pool,
            vk::BufferUsageFlags destinationUsage);

        void PushSourceBytes(Bytes bytes, vk::DeviceSize offset);
        void PushSourceBytes(const void* bytes, vk::DeviceSize offset, size_t size);
        template<class T>
        void PushSourceBytes(const T& object, vk::DeviceSize offset);
        Command CopyFromSourceToDestination();
        Command CopyFromSourceToDestination(vk::DeviceSize offset, vk::DeviceSize size);
    private:
        struct Internal {};

        StagedBuffer(
            Chroma::TypeIdentity<Internal>,
            vk::DeviceSize size,
            vk::Device device,
            MemoryPool& pool,
            std::optional<vk::BufferUsageFlags> destinationUsage);
    };

    template<class T>
    void StagedBuffer::PushSourceBytes(const T& object, vk::DeviceSize offset)
    {
        source.PushBytes(object, offset);
    }
}