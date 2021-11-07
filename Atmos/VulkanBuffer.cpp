#include "VulkanBuffer.h"

#include "VulkanMemoryPool.h"

namespace Atmos::Render::Vulkan
{
    Buffer::Buffer(
        vk::DeviceSize size,
        vk::BufferUsageFlags usage,
        vk::MemoryPropertyFlags flags,
        vk::Device device,
        MemoryPool& pool)
        :
        size(size)
    {
        const vk::BufferCreateInfo createInfo(
            {},
            size,
            usage,
            vk::SharingMode::eExclusive);

        value = device.createBufferUnique(createInfo);

        memory = pool.Bind(value.get(), flags);
    }

    void Buffer::PushBytes(const Bytes& bytes, vk::DeviceSize offset)
    {
        PushBytes(bytes.data(), offset, bytes.size());
    }

    void Buffer::PushBytes(const void* bytes, vk::DeviceSize offset, size_t size)
    {
        memory.PushBytes(bytes, offset, size);
    }

    Command Buffer::Copy(
        const Buffer& destination,
        vk::DeviceSize sourceOffset,
        vk::DeviceSize destinationOffset,
        vk::DeviceSize size) const
    {
        if (size == 0)
            return {};
        else
            return [this, &destination, sourceOffset, destinationOffset, size](vk::CommandBuffer commandBuffer)
            {
                const vk::BufferCopy copyRegion(sourceOffset, destinationOffset, size);
                commandBuffer.copyBuffer(value.get(), destination.value.get(), copyRegion);
            };
    }

    Command Buffer::Copy(
        vk::Image destination,
        vk::DeviceSize bufferOffset,
        uint32_t bufferRowLength,
        uint32_t bufferImageHeight,
        vk::Offset3D imagePosition,
        vk::Extent3D imageSize,
        uint32_t layerOffset,
        uint32_t layerCount)
    {
        return [this, destination, bufferOffset, bufferRowLength, bufferImageHeight, imagePosition, imageSize, layerOffset, layerCount]
        (vk::CommandBuffer commandBuffer)
        {
            const vk::ImageSubresourceLayers subresourceLayers(
                vk::ImageAspectFlagBits::eColor, 0, layerOffset, layerCount);
            const vk::BufferImageCopy region(
                bufferOffset,
                bufferRowLength,
                bufferImageHeight,
                subresourceLayers,
                imagePosition,
                imageSize);

            commandBuffer.copyBufferToImage(
                value.get(), destination, vk::ImageLayout::eTransferDstOptimal, region);
        };
    }

    vk::Buffer Buffer::Value() const
    {
        return value.get();
    }

    vk::DeviceSize Buffer::Size() const
    {
        return size;
    }
}