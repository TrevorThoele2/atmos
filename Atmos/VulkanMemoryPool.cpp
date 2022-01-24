#include "VulkanMemoryPool.h"

#include "VulkanResults.h"
#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    MemoryPool::MemoryPool(vk::DeviceSize minBlockSize, vk::PhysicalDeviceMemoryProperties properties, vk::Device device) :
        properties(properties), minBlockSize(minBlockSize), device(device)
    {}

    UniqueMemory MemoryPool::Bind(vk::Buffer buffer, vk::MemoryPropertyFlags flags)
    {
        const auto memoryRequirements = device.getBufferMemoryRequirements(buffer);

        const auto block = Suballocate(
            memoryRequirements.size,
            memoryRequirements.alignment,
            SuitableMemoryType(memoryRequirements.memoryTypeBits, flags));
        device.bindBufferMemory(buffer, block.memory, block.offset);
        return ToMemory(block);
    }

    UniqueMemory MemoryPool::Bind(vk::Image image, vk::MemoryPropertyFlags flags)
    {
        const auto memoryRequirements = device.getImageMemoryRequirements(image);

        const auto block = Suballocate(
            memoryRequirements.size,
            memoryRequirements.alignment,
            SuitableMemoryType(memoryRequirements.memoryTypeBits, flags));

        device.bindImageMemory(image, block.memory, block.offset);
        return ToMemory(block);
    }

    void MemoryPool::PushBytes(size_t allocation, size_t reservation, const void* bytes, vk::DeviceSize offset, size_t size)
    {
        auto& foundAllocation = *FindAllocation(allocation);
        const auto& foundReservation = *FindReservation(reservation, foundAllocation);
        const auto memory = foundAllocation.memory.get();

        void* data;
        if (IsError(device.mapMemory(memory, foundReservation.offset + offset, vk::DeviceSize(size), vk::MemoryMapFlags{}, &data)))
            throw GraphicsError("Could not map memory in Vulkan buffer.");
        memcpy(data, bytes, size);
        device.unmapMemory(memory);
    }

    void MemoryPool::Return(size_t allocation, size_t reservation)
    {
        auto foundAllocation = FindAllocation(allocation);
        if (foundAllocation != allocations.end())
        {
            const auto foundReservation = FindReservation(reservation, *foundAllocation);
            if (foundReservation != foundAllocation->reservations.end())
            {
                foundAllocation->freedReservationIDs.emplace(foundReservation->id);
                foundAllocation->reservations.erase(foundReservation);
            }
        }
    }
    
    auto MemoryPool::Suballocate(vk::DeviceSize size, vk::DeviceSize alignment, uint32_t memoryType) -> Block
    {
        const auto found = AttemptReserve(size, alignment, memoryType);
        return found ? *found : Allocate(size, memoryType);
    }

    auto MemoryPool::Allocate(vk::DeviceSize reservationSize, uint32_t memoryType) -> Block
    {
        const auto allocationSize = std::max(reservationSize, minBlockSize);
        
        auto memory = device.allocateMemoryUnique(vk::MemoryAllocateInfo{ allocationSize, memoryType });

        constexpr auto reservationID = 1;
        const auto reservation = Reservation{ reservationID, 0, reservationSize };
        const auto allocationID = allocations.size() + 1;
        allocations.push_back(Allocation{ allocationID, std::move(memory), allocationSize, memoryType, {reservation} });
        auto& allocation = allocations.back();
        return { allocationID, reservationID, allocation.memory.get(), 0 };
    }

    auto MemoryPool::AttemptReserve(vk::DeviceSize size, vk::DeviceSize alignment, uint32_t memoryType) -> std::optional<Block>
    {
        for (auto& allocation : allocations)
        {
            if (allocation.memoryType != memoryType)
                continue;
            
            auto reservations = std::vector{ Reservation{0, 0, 0} };
            reservations.insert(reservations.end(), allocation.reservations.begin(), allocation.reservations.end());
            reservations.emplace_back(0, allocation.size, 0);
            std::ranges::sort(reservations, [](const Reservation& left, const Reservation& right) { return left.offset < right.offset; });

            auto leftReservation = reservations.begin();
            auto rightReservation = reservations.begin() + 1;
            for (; rightReservation != reservations.end(); ++rightReservation, ++leftReservation)
            {
                const auto left = leftReservation->offset + leftReservation->size;
                const auto alignedLeft = alignment != 0
                    ? static_cast<vk::DeviceSize>(ceil(static_cast<double>(left) / static_cast<double>(alignment)) * alignment)
                    : left;
                const auto right = rightReservation->offset;
                if (alignedLeft < right && right - alignedLeft >= size)
                {
                    const auto reservationID = NextReservationID(allocation);
                    allocation.reservations.emplace_back(reservationID, alignedLeft, size);
                    return Block{ allocation.id, reservationID, allocation.memory.get(), alignedLeft };
                }
            }
        }

        return {};
    }

    auto MemoryPool::FindAllocation(size_t id) -> std::vector<Allocation>::iterator
    {
        for (auto allocation = allocations.begin(); allocation != allocations.end(); ++allocation)
            if (allocation->id == id)
                return allocation;

        return allocations.end();
    }

    auto MemoryPool::FindReservation(size_t id, Allocation& allocation) -> std::vector<Reservation>::iterator
    {
        for (auto reservation = allocation.reservations.begin(); reservation != allocation.reservations.end(); ++reservation)
            if (reservation->id == id)
                return reservation;

        return allocation.reservations.end();
    }
    
    uint32_t MemoryPool::SuitableMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags flags)
    {
        for (uint32_t i = 0; i < properties.memoryTypeCount; ++i)
            if (typeFilter & (1 << i) && (properties.memoryTypes[i].propertyFlags & flags) == flags)
                return i;

        throw GraphicsError("Could not find suitable memory type.");
    }
    
    UniqueMemory MemoryPool::ToMemory(const Block& block)
    {
        return { block.allocationID, block.reservationID, *this };
    }

    size_t MemoryPool::NextReservationID(Allocation& allocation)
    {
        if (!allocation.freedReservationIDs.empty())
        {
            const auto itr = allocation.freedReservationIDs.begin();
            const auto id = *itr;
            allocation.freedReservationIDs.erase(itr);
            return id;
        }
        else
            return allocation.reservations.size() + 1;
    }
}