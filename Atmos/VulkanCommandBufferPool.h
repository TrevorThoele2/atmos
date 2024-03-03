#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class CommandBufferPool
    {
    public:
        CommandBufferPool(vk::Device device, uint32_t queueFamily, vk::CommandPoolCreateFlags flags);
        
        void Reserve(uint32_t count);
        void Reset();
        
        [[nodiscard]] vk::CommandBuffer Checkout();
        void Return(vk::CommandBuffer commandBuffer);

        [[nodiscard]] size_t Size() const;
    private:
        vk::UniqueCommandPool pool;
        std::vector<vk::CommandBuffer> available;
        std::vector<vk::UniqueCommandBuffer> all;
    private:
        vk::Device device;
    };
}