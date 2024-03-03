#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class Buffer
    {
    public:
        struct Init
        {
            vk::DeviceSize size;
            vk::BufferUsageFlags usage;
            vk::MemoryPropertyFlags properties;
            vk::Device device;
            vk::PhysicalDeviceMemoryProperties memoryProperties;
        };
    public:
        vk::UniqueBuffer value;
        vk::UniqueDeviceMemory memory;
        vk::DeviceSize size = 0;

        Buffer(
            vk::DeviceSize size,
            vk::BufferUsageFlags usage,
            vk::MemoryPropertyFlags properties,
            vk::Device device,
            vk::PhysicalDeviceMemoryProperties memoryProperties);
        Buffer(const Buffer& arg) = delete;
        Buffer(Buffer&& arg) noexcept = default;
        Buffer& operator=(const Buffer& arg) = delete;
        Buffer& operator=(Buffer&& arg) noexcept = default;

        void PushBytes(Bytes bytes, vk::DeviceSize offset);
        void PushBytes(const void* bytes, vk::DeviceSize offset, size_t size);
        template<class T>
        void PushBytes(const T& object, vk::DeviceSize offset);
        void Copy(
            const Buffer& destination,
            vk::DeviceSize sourceOffset,
            vk::DeviceSize destinationOffset,
            vk::DeviceSize size,
            vk::CommandPool commandPool,
            vk::Queue queue) const;
        void Copy(
            vk::Image destination,
            vk::DeviceSize bufferOffset,
            uint32_t bufferRowLength,
            uint32_t bufferImageHeight,
            vk::Offset3D imagePosition,
            vk::Extent3D imageSize,
            uint32_t layerOffset,
            uint32_t layerCount,
            vk::CommandPool commandPool,
            vk::Queue queue);
    private:
        vk::Device device;
    };

    template<class T>
    void Buffer::PushBytes(const T& object, vk::DeviceSize offset)
    {
        PushBytes(static_cast<const void*>(&object), offset, sizeof T);
    }
}