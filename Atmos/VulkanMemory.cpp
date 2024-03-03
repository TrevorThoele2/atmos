#include "VulkanMemory.h"

#include "VulkanMemoryPool.h"

namespace Atmos::Render::Vulkan
{
    UniqueMemory::UniqueMemory() : allocation(0), reservation(0), pool(nullptr)
    {}

    UniqueMemory::UniqueMemory(size_t allocation, size_t reservation, MemoryPool& pool) :
        allocation(allocation), reservation(reservation), pool(&pool)
    {}

    UniqueMemory::UniqueMemory(UniqueMemory&& arg) noexcept :
        allocation(arg.allocation), reservation(arg.reservation), pool(arg.pool)
    {
        arg.allocation = 0;
        arg.reservation = 0;
        arg.pool = nullptr;
    }

    UniqueMemory::~UniqueMemory()
    {
        if (pool)
            pool->Return(allocation, reservation);
    }

    UniqueMemory& UniqueMemory::operator=(UniqueMemory&& arg) noexcept
    {
        allocation = arg.allocation;
        reservation = arg.reservation;
        pool = arg.pool;
        arg.allocation = 0;
        arg.reservation = 0;
        arg.pool = nullptr;
        return *this;
    }
    
    void UniqueMemory::PushBytes(const void* bytes, vk::DeviceSize offset, size_t size)
    {
        pool->PushBytes(allocation, reservation, bytes, offset, size);
    }
}