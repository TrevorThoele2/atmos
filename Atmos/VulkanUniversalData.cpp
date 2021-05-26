#include "VulkanUniversalData.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Atmos::Render::Vulkan
{
    UniversalData::UniversalData(const glm::vec2& screenSize, const glm::vec2& mapPosition) :
        mapPosition(mapPosition), screenSize(screenSize)
    {
        const auto left = 0.0f;
        const auto top = 0.0f;
        const auto right = screenSize.x;
        const auto bottom = screenSize.y;

        const glm::vec3 eye = { 0.0f, 0.0f, 1.0f };
        const glm::vec3 center = { 0.0f, 0.0f, 0.0f };
        const glm::vec3 up = { 0.0f, 1.0f, 0.0f };

        model = glm::mat4(1.0f);
        view = glm::lookAt(eye, center, up);
        projection = glm::ortho(left, right, top, bottom, 1.0f, 0.0f);
    }
}