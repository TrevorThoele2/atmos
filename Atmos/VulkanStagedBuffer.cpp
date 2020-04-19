#include "VulkanStagedBuffer.h"

namespace Atmos::Render::Vulkan
{
    StagedBuffer::StagedBuffer(
        vk::DeviceSize size,
        vk::Device device,
        vk::PhysicalDeviceMemoryProperties memoryProperties)
        :
        StagedBuffer({}, size, device, memoryProperties, {})
    {}

    StagedBuffer::StagedBuffer(
        vk::DeviceSize size,
        vk::Device device,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        vk::BufferUsageFlags destinationUsage)
        :
        StagedBuffer({}, size, device, memoryProperties, destinationUsage)
    {}

    void StagedBuffer::PushSourceBytes(Bytes bytes, vk::DeviceSize offset)
    {
        source.PushBytes(bytes, offset);
    }

    void StagedBuffer::PushSourceBytes(const void* bytes, vk::DeviceSize offset, size_t size)
    {
        source.PushBytes(bytes, offset, size);
    }

    void StagedBuffer::CopyFromSourceToDestination(
        vk::DeviceSize size, vk::CommandPool commandPool, vk::Queue queue)
    {
        source.Copy(destination, size, commandPool, queue);
    }

    void StagedBuffer::CopyFromSourceToDestination(vk::CommandPool commandPool, vk::Queue queue)
    {
        source.Copy(destination, source.size, commandPool, queue);
    }

    StagedBuffer::StagedBuffer(
        Chroma::TypeIdentity<Internal>,
        vk::DeviceSize size,
        vk::Device device,
        vk::PhysicalDeviceMemoryProperties memoryProperties,
        std::optional<vk::BufferUsageFlags> destinationUsage)
        :
        source(
            size,
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            device,
            memoryProperties),
        destination(
            size,
            destinationUsage
                ? vk::BufferUsageFlagBits::eTransferDst | *destinationUsage
                : vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            device,
            memoryProperties)
    {}
}