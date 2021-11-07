#include "VulkanCommandBufferPool.h"

#include "VulkanCommandBuffer.h"

namespace Atmos::Render::Vulkan
{
    CommandBufferPool::CommandBufferPool(vk::Device device, uint32_t queueFamily, vk::CommandPoolCreateFlags flags) :
        device(device)
    {
        const vk::CommandPoolCreateInfo commandPoolCreateInfo(flags, queueFamily);
        pool = device.createCommandPoolUnique(commandPoolCreateInfo);
    }
    
    void CommandBufferPool::Reserve(uint32_t count)
    {
        if (count > all.size())
        {
            const auto allocateCount = count - all.size();
            auto newBuffers = CreateCommandBuffers(device, pool.get(), vk::CommandBufferLevel::ePrimary, allocateCount);
            for (auto& buffer : newBuffers)
            {
                available.push_back(buffer.get());
                all.push_back(std::move(buffer));
            }
        }
    }

    void CommandBufferPool::Reset()
    {
        pool.reset();
    }
    
    vk::CommandBuffer CommandBufferPool::Checkout()
    {
        if (available.empty())
            Reserve(all.size() + 1);

        const auto returnValue = available[available.size() - 1];
        available.pop_back();
        return returnValue;
    }

    void CommandBufferPool::Return(vk::CommandBuffer commandBuffer)
    {
        available.push_back(commandBuffer);
    }

    size_t CommandBufferPool::Size() const
    {
        return all.size();
    }
}