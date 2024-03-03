#include "VulkanRenderPass.h"

#include "VulkanOperation.h"

namespace Atmos::Render::Vulkan
{
    void RenderPass(vk::RenderPassBeginInfo beginInfo, vk::SubpassContents contents, vk::CommandBuffer commandBuffer, const std::function<void()>& function)
    {
        Operation(
            [beginInfo, contents, commandBuffer]()
            {
                commandBuffer.beginRenderPass(beginInfo, contents);
            },
            [&function]()
            {
                function();
            },
            [&commandBuffer]()
            {
                commandBuffer.endRenderPass();
            });
    }
}