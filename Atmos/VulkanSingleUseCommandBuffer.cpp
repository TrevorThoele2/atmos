#include "VulkanSingleUseCommandBuffer.h"

namespace Atmos::Render::Vulkan
{
    void SingleUseCommandBuffer(
        vk::Device device,
        vk::CommandPool commandPool,
        vk::Queue queue,
        const std::function<void(vk::CommandBuffer)>& execute)
    {
        const vk::CommandBufferAllocateInfo allocateInfo(commandPool, vk::CommandBufferLevel::ePrimary, 1);
        auto commandBuffer = std::move(device.allocateCommandBuffersUnique(allocateInfo)[0]);

        const vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffer->begin(beginInfo);

        execute(commandBuffer.get());

        commandBuffer->end();

        const vk::SubmitInfo submitInfo({}, {}, {}, 1, &commandBuffer.get());
        queue.submit(submitInfo, nullptr);
        queue.waitIdle();
    }
}