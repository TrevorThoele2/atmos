#include "VulkanUtilities.h"

namespace Atmos::Render::Vulkan
{
    glm::vec4 AtmosToVulkanColor(const Color& color)
    {
        constexpr auto converter = [](Color::Value input)
        {
            constexpr auto atmosDifferential = std::numeric_limits<Color::Value>::max();
            constexpr auto vulkanDifferential = 1.0f;
            constexpr auto atmosMin = std::uint8_t(0);
            constexpr auto vulkanMin = 0.0f;
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