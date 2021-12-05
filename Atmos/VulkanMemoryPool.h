#pragma once

#include <optional>
#include <set>
#include "VulkanIncludes.h"
#include "VulkanMemory.h"
#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    class MemoryPool
    {
    public:
        MemoryPool(vk::DeviceSize minBlockSize, vk::PhysicalDeviceMemoryProperties properties, vk::Device device);

        [[nodiscard]] UniqueMemory Bind(vk::Buffer buffer, vk::MemoryPropertyFlags flags);
        [[nodiscard]] UniqueMemory Bind(vk::Image image, vk::MemoryPropertyFlags flags);
        
        void PushBytes(size_t allocation, size_t reservation, const void* bytes, vk::DeviceSize offset, size_t size);

        void Return(size_t allocation, size_t reservation);
    private:
        vk::PhysicalDeviceMemoryProperties properties;
        vk::DeviceSize minBlockSize;

        struct Reservation
        {
            size_t id;
            vk::DeviceSize offset;
            vk::DeviceSize size;
        };

        struct Allocation
        {
            size_t id;
            vk::UniqueDeviceMemory memory;
            vk::DeviceSize size;
            uint32_t memoryType;
            std::vector<Reservation> reservations;
            std::set<size_t> freedReservationIDs;
        };
        std::vector<Allocation> allocations;

        struct Block
        {
            size_t allocationID;
            size_t reservationID;
            vk::DeviceMemory memory;
            vk::DeviceSize offset;
        };
        
        [[nodiscard]] Block Suballocate(vk::DeviceSize size, vk::DeviceSize alignment, uint32_t memoryType);
        [[nodiscard]] Block Allocate(vk::DeviceSize reservationSize, uint32_t memoryType);
        [[nodiscard]] std::optional<Block> AttemptReserve(vk::DeviceSize size, vk::DeviceSize alignment, uint32_t memoryType);

        [[nodiscard]] std::vector<Allocation>::iterator FindAllocation(size_t id);
        [[nodiscard]] std::vector<Reservation>::iterator FindReservation(size_t id, Allocation& allocation);

        [[nodiscard]] uint32_t SuitableMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags flags);

        [[nodiscard]] UniqueMemory ToMemory(const Block& block);

        [[nodiscard]] static size_t NextReservationID(Allocation& allocation);
    private:
        vk::Device device;
    };
}