#pragma once

#include "VulkanIncludes.h"
#include "VulkanCommand.h"
#include "VulkanMemory.h"

namespace Atmos::Render::Vulkan
{
    class MemoryPool;

    class Buffer
    {
    public:
        Buffer(
            vk::DeviceSize size,
            vk::BufferUsageFlags usage,
            vk::MemoryPropertyFlags flags,
            vk::Device device,
            MemoryPool& pool);
        Buffer(const Buffer& arg) = delete;
        Buffer(Buffer&& arg) noexcept = default;
        Buffer& operator=(const Buffer& arg) = delete;
        Buffer& operator=(Buffer&& arg) noexcept = default;

        void PushBytes(const Bytes& bytes, vk::DeviceSize offset);
        void PushBytes(const void* bytes, vk::DeviceSize offset, size_t size);
        template<class T>
        void PushBytes(const T& object, vk::DeviceSize offset);
        [[nodiscard]] Command Copy(
            const Buffer& destination,
            vk::DeviceSize sourceOffset,
            vk::DeviceSize destinationOffset,
            vk::DeviceSize size) const;
        [[nodiscard]] Command Copy(
            vk::Image destination,
            vk::DeviceSize bufferOffset,
            uint32_t bufferRowLength,
            uint32_t bufferImageHeight,
            vk::Offset3D imagePosition,
            vk::Extent3D imageSize,
            uint32_t layerOffset,
            uint32_t layerCount);

        [[nodiscard]] vk::Buffer Value() const;
        [[nodiscard]] vk::DeviceSize Size() const;
    private:
        vk::UniqueBuffer value;
        UniqueMemory memory;
        vk::DeviceSize size = 0;
    };

    template<class T>
    void Buffer::PushBytes(const T& object, vk::DeviceSize offset)
    {
        PushBytes(static_cast<const void*>(&object), offset, sizeof T);
    }
}