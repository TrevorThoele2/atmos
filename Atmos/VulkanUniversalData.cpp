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

        /*
        const auto left = cameraSides.x;
        const auto top = cameraSides.y;
        const auto right = cameraSides.z;
        const auto bottom = cameraSides.w;
        */

        const glm::vec3 eye = { 0.0f, 0.0f, 1.0f };
        const glm::vec3 center = { 0.0f, 0.0f, 0.0f };
        const glm::vec3 up = { 0.0f, 1.0f, 0.0f };

        /*
        const float cameraCenter[] = { (left + right) / 2.0f, (top + bottom) / 2.0f };

        const glm::vec3 eye = { cameraCenter[0], cameraCenter[1], 1.0f };
        const glm::vec3 center = { cameraCenter[0], cameraCenter[1], 0.0f };
        const glm::vec3 up = { 0.0f, 1.0f, 0.0f };
        */

        auto data = UniversalData
        {
            glm::mat4(1.0f),
            glm::lookAt(eye, center, up),
            glm::ortho(left, right, bottom, top, 1.0f, 0.0f)
        };
        //data.projection[0][0] *= -1;
        //data.projection[1][1] *= -1;
        return data;
    }
}