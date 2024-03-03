#include "VulkanUtilities.h"

namespace Atmos::Render::Vulkan
{
    bool IsSuccess(vk::Result result)
    {
        return result == vk::Result::eSuccess;
    }

    bool IsSuccess(VkResult result)
    {
        return result == VK_SUCCESS;
    }

    bool IsError(vk::Result result)
    {
        return result != vk::Result::eSuccess;
    }

    bool IsError(VkResult result)
    {
        return result != VK_SUCCESS;
    }

    glm::vec4 AtmosToVulkanColor(const Color& color)
    {
        const auto converter = [](Color::Value input)
        {
            const auto atmosDifferential = std::numeric_limits<Color::Value>::max();
            const auto vulkanDifferential = 1.0f;
            const auto atmosMin = std::uint8_t(0);
            const auto vulkanMin = 0.0f;
            return (input - atmosMin) * vulkanDifferential / atmosDifferential + vulkanMin;
        };

        return
        {
            converter(color.red),
            converter(color.green),
            converter(color.blue),
            converter(color.alpha)
        };
    }
}