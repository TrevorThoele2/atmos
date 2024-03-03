#include "VulkanBuffer.h"

#include "VulkanMemory.h"
#include "VulkanSingleUseCommandBuffer.h"
#include "VulkanUtilities.h"
#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    Buffer::Buffer(
        vk::DeviceSize size,
        vk::BufferUsageFlags usage,
        vk::MemoryPropertyFlags properties,
        vk::Device device,
        vk::PhysicalDeviceMemoryProperties memoryProperties)
        :
        size(size),
        device(device)
    {
        const vk::BufferCreateInfo createInfo(
            {},
            size,
            usage,
            vk::SharingMode::eExclusive);

        value = device.createBufferUnique(createInfo);

        const auto memoryRequirements = device.getBufferMemoryRequirements(value.get());

        const vk::MemoryAllocateInfo memoryAllocationInfo(
            memoryRequirements.size,
            FindSuitableMemoryType(
                memoryRequirements.memoryTypeBits,
                properties,
                memoryProperties));

        memory = device.allocateMemoryUnique(memoryAllocationInfo);

        device.bindBufferMemory(value.get(), memory.get(), 0);
    }

    void Buffer::PushBytes(Bytes bytes, vk::DeviceSize offset)
    {
        PushBytes(bytes.data(), offset, bytes.size());
    }

    void Buffer::PushBytes(const void* bytes, vk::DeviceSize offset, size_t size)
    {
        void* data;
        if (IsError(device.mapMemory(memory.get(), offset, vk::DeviceSize(size), vk::MemoryMapFlags{}, &data)))
            throw GraphicsError("Could not map memory in Vulkan buffer.");
        memcpy(data, bytes, size);
        device.unmapMemory(memory.get());
    }

    void Buffer::Copy(
        const Buffer& destination,
        vk::DeviceSize sourceOffset,
        vk::DeviceSize destinationOffset,
        vk::DeviceSize size,
        vk::CommandPool commandPool,
        vk::Queue queue) const
    {
        if (size == 0)
            return;

        SingleUseCommandBuffer(device, commandPool, queue,
            [this, &destination, sourceOffset, destinationOffset, size](vk::CommandBuffer commandBuffer)
            {
                const vk::BufferCopy copyRegion(sourceOffset, destinationOffset, size);
                commandBuffer.copyBuffer(value.get(), destination.value.get(), copyRegion);
            });
    }

    void Buffer::Copy(
        vk::Image destination,
        vk::DeviceSize bufferOffset,
        uint32_t bufferRowLength,
        uint32_t bufferImageHeight,
        vk::Offset3D imagePosition,
        vk::Extent3D imageSize,
        uint32_t layerOffset,
        uint32_t layerCount,
        vk::CommandPool commandPool,
        vk::Queue queue)
    {
        SingleUseCommandBuffer(device, commandPool, queue,
            [
                this,
                destination,
                bufferOffset,
                bufferRowLength,
                bufferImageHeight,
                imagePosition,
                imageSize,
                layerOffset,
                layerCount
            ]
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
            });
    }
}