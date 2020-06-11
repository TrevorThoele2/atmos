#pragma once

#include "VulkanIncludes.h"

#include "Position3D.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    class RendererInterface
    {
    public:
        virtual ~RendererInterface() = 0;

        virtual void DrawNextLayer(uint32_t currentImage, glm::vec2 cameraSize) = 0;

        [[nodiscard]] virtual bool IsDone() const = 0;
        [[nodiscard]] virtual Position3D::Value NextLayer() const = 0;
    };
}