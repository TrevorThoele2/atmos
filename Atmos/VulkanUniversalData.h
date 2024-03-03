#pragma once

#include <glm/glm.hpp>
#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    struct UniversalData
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        glm::vec2 mapPosition;
        glm::vec2 screenSize;

        UniversalData(const glm::vec2& screenSize, const glm::vec2& mapPosition);
    };
}