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

        static UniversalData Ortho(const glm::vec2& cameraSize);
    };
}