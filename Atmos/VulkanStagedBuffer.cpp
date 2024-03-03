#include "VulkanStagedBuffer.h"

namespace Atmos::Render::Vulkan
{
    StagedBuffer::StagedBuffer(
        vk::DeviceSize size,
        vk::Device device,
        MemoryPool& pool)
        :
        StagedBuffer({}, size, device, pool, {})
    {}

    StagedBuffer::StagedBuffer(
        vk::DeviceSize size,
        vk::Device device,
        MemoryPool& pool,
        vk::BufferUsageFlags destinationUsage)
        :
        StagedBuffer({}, size, device, pool, destinationUsage)
    {}

    void StagedBuffer::PushSourceBytes(Bytes bytes, vk::DeviceSize offset)
    {
        source.PushBytes(bytes, offset);
    }

    void StagedBuffer::PushSourceBytes(const void* bytes, vk::DeviceSize offset, size_t size)
    {
        source.PushBytes(bytes, offset, size);
    }

    Command StagedBuffer::CopyFromSourceToDestination()
    {
        return source.Copy(destination, 0, 0, source.Size());
    }

    Command StagedBuffer::CopyFromSourceToDestination(
        vk::DeviceSize offset, vk::DeviceSize size)
    {
        return source.Copy(destination, offset, offset, size);
    }

    StagedBuffer::StagedBuffer(
        Chroma::TypeIdentity<Internal>,
        vk::DeviceSize size,
        vk::Device device,
        MemoryPool& pool,
        std::optional<vk::BufferUsageFlags> destinationUsage)
        :
        source(
            size,
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            device,
            pool),
        destination(
            size,
            destinationUsage
                ? vk::BufferUsageFlagBits::eTransferDst | *destinationUsage
                : vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            device,
            pool)
    {}
}