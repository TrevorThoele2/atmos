#include "VulkanCommandBufferPool.h"

namespace Atmos::Render::Vulkan
{
    CommandBufferPool::CommandBufferPool(vk::Device device, uint32_t queueFamily) :
        device(device)
    {
        const vk::CommandPoolCreateInfo commandPoolCreateInfo(
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            queueFamily);
        pool = device.createCommandPoolUnique(commandPoolCreateInfo);
    }

    vk::CommandBuffer CommandBufferPool::operator[](uint32_t index) const
    {
        return At(index);
    }

    void CommandBufferPool::Reserve(uint32_t count)
    {
        if (count <= allBuffers.size())
            return;

        const auto allocateCount = count - allBuffers.size();
        const vk::CommandBufferAllocateInfo allocateInfo(
            pool.get(), vk::CommandBufferLevel::ePrimary, allocateCount);
        auto newBuffers = device.allocateCommandBuffersUnique(allocateInfo);
        for (auto& buffer : newBuffers)
        {
            availableBuffers.push_back(buffer.get());
            allBuffers.push_back(std::move(buffer));
        }
    }

    void CommandBufferPool::DoneWith(vk::CommandBuffer commandBuffer)
    {
        availableBuffers.push_back(commandBuffer);
    }

    vk::CommandBuffer CommandBufferPool::At(uint32_t index) const
    {
        return allBuffers[index].get();
    }

    vk::CommandBuffer CommandBufferPool::Next()
    {
        if (availableBuffers.empty())
            Reserve(allBuffers.size() + 1);

        const auto returnValue = availableBuffers[availableBuffers.size() - 1];
        availableBuffers.pop_back();
        return returnValue;
    }

    size_t CommandBufferPool::Size() const
    {
        return allBuffers.size();
    }

    vk::CommandPool CommandBufferPool::Pool() const
    {
        return pool.get();
    }
}