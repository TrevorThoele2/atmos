#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class MemoryPool;

    class UniqueMemory final
    {
    public:
        UniqueMemory();
        UniqueMemory(size_t allocation, size_t reservation, MemoryPool& pool);
        UniqueMemory(const UniqueMemory& arg) = delete;
        UniqueMemory(UniqueMemory&& arg) noexcept;

        ~UniqueMemory();

        UniqueMemory& operator=(UniqueMemory&& arg) noexcept;
        
        void PushBytes(const void* bytes, vk::DeviceSize offset, size_t size);
    private:
        size_t allocation;
        size_t reservation;
        MemoryPool* pool;
    };
}