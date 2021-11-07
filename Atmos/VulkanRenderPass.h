#pragma once

#include "VulkanCommandBuffer.h"

namespace Atmos::Render::Vulkan
{
	void RenderPass(vk::RenderPassBeginInfo beginInfo, vk::SubpassContents contents, vk::CommandBuffer commandBuffer, const std::function<void()>& function);
}