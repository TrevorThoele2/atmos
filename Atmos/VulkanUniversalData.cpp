#include "VulkanUniversalData.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Atmos::Render::Vulkan
{
    UniversalData UniversalData::Ortho(const glm::vec2& cameraSize)
    {
        const auto left = 0.0f;
        const auto top = 0.0f;
        const auto right = cameraSize.x;
        const auto bottom = cameraSize.y;

        const glm::vec3 eye = { 0.0f, 0.0f, 1.0f };
        const glm::vec3 center = { 0.0f, 0.0f, 0.0f };
        const glm::vec3 up = { 0.0f, 1.0f, 0.0f };

        auto data = UniversalData
        {
            glm::mat4(1.0f),
            glm::lookAt(eye, center, up),
            glm::ortho(left, right, top, bottom, 1.0f, 0.0f)
        };

        return data;
    }
}