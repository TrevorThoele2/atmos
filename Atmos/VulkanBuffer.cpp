#include "VulkanBuffer.h"

#include "VulkanMemory.h"
#include "VulkanSingleUseCommandBuffer.h"
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
        device.mapMemory(memory.get(), offset, vk::DeviceSize(size), vk::MemoryMapFlags{}, &data);
        memcpy(data, bytes, size);
        device.unmapMemory(memory.get());
    }

    void Buffer::Copy(const Buffer& destination, vk::DeviceSize size, vk::CommandPool commandPool, vk::Queue queue) const
    {
        if (size == 0)
            return;

        SingleUseCommandBuffer(device, commandPool, queue,
            [this, &destination, size](vk::CommandBuffer commandBuffer)
            {
                const vk::BufferCopy copyRegion(0, 0, size);
                commandBuffer.copyBuffer(value.get(), destination.value.get(), copyRegion);
            });
    }

    void Buffer::Copy(vk::Image destination, uint32_t width, uint32_t height, vk::CommandPool commandPool, vk::Queue queue)
    {
        SingleUseCommandBuffer(device, commandPool, queue,
            [this, destination, width, height](vk::CommandBuffer commandBuffer)
            {
                const vk::ImageSubresourceLayers subresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
                const vk::BufferImageCopy region(
                    {}, {}, {}, subresourceLayers, { 0, 0, 0 }, { width, height, 1 });

                commandBuffer.copyBufferToImage(
                    value.get(), destination, vk::ImageLayout::eTransferDstOptimal, region);
            });
    }
}