#pragma once

#include "VulkanIncludes.h"

#include "Color.h"
#include "String.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    glm::vec4 ToVulkanColor(const Color& color);

    template<size_t i>
    String ToString(vk::ArrayWrapper1D<char, i> input)
    {
        String string;
        for (auto character : input)
            string.push_back(character);
        return string;
    }
}