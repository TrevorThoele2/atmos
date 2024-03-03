#pragma once

#include "VulkanIncludes.h"
#include "VulkanCommand.h"

namespace Atmos::Render::Vulkan
{
    std::vector<vk::UniqueCommandBuffer> CreateCommandBuffers(
        vk::Device device, vk::CommandPool commandPool, vk::CommandBufferLevel bufferLevel, size_t count);
    
    class CommandRecorder
    {
    public:
        explicit CommandRecorder(vk::CommandBuffer underlying);
        void operator()(const Command& command);

        [[nodiscard]] vk::CommandBuffer Buffer() const;
    private:
        vk::CommandBuffer underlying;
    };

    void RecordAndSubmit(
        vk::CommandBuffer to,
        vk::CommandBufferUsageFlags flags,
        vk::Queue queue,
        vk::Fence fence,
        const std::function<void(CommandRecorder&)>& apply);
}