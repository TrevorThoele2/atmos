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
        void Copy(const Buffer& destination, vk::DeviceSize size, vk::CommandPool commandPool, vk::Queue queue) const;
        void Copy(vk::Image destination, uint32_t width, uint32_t height, vk::CommandPool commandPool, vk::Queue queue);
    private:
        vk::Device device;
    };

    template<class T>
    void Buffer::PushBytes(const T& object, vk::DeviceSize offset)
    {
        PushBytes(static_cast<const void*>(&object), offset, sizeof T);
    }
}