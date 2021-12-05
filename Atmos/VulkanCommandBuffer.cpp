#include "VulkanCommandBuffer.h"

#include "GraphicsError.h"

namespace Atmos::Render::Vulkan
{
    std::vector<vk::UniqueCommandBuffer> CreateCommandBuffers(
        vk::Device device, vk::CommandPool commandPool, vk::CommandBufferLevel bufferLevel, size_t count)
    {
        const vk::CommandBufferAllocateInfo allocateInfo(commandPool, bufferLevel, count);
        auto commandBuffers = device.allocateCommandBuffersUnique(allocateInfo);
        return commandBuffers;
    }
    
    CommandRecorder::CommandRecorder(vk::CommandBuffer underlying) : underlying(underlying)
    {}

    void CommandRecorder::operator()(const Command& command)
    {
        if (command.index() == 0)
            std::get<0>(command)(underlying);
        else if (command.index() == 1)
            std::get<1>(command)(*this);
        else
            throw GraphicsError("Unknown command index.");
    }

	vk::CommandBuffer CommandRecorder::Buffer() const
    {
        return underlying;
    }

    void Record(
        vk::CommandBuffer to,
        vk::CommandBufferUsageFlags flags,
        const std::function<void(CommandRecorder&)>& apply)
    {
        const vk::CommandBufferBeginInfo beginInfo(flags);
        to.begin(beginInfo);

        auto recorder = CommandRecorder(to);
        apply(recorder);

        to.end();
    }

    void Submit(
        vk::CommandBuffer to,
        vk::Queue queue,
        vk::Fence fence)
    {
        vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &to, 0, nullptr);
        queue.submit(submitInfo, fence);
    }
}