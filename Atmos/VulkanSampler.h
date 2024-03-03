#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
	[[nodiscard]] vk::UniqueSampler CreateSampler(vk::Device device);
}