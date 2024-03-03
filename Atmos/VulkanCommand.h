#pragma once

#include <functional>
#include <variant>
#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class CommandRecorder;
    using Command = std::variant<std::function<void(vk::CommandBuffer)>, std::function<void(CommandRecorder)>>;
}