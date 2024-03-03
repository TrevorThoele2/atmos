#pragma once

#include <glm/glm.hpp>
#include "Color.h"
#include "Point2D.h"
#include "Size2D.h"
#include "Scalers2D.h"
#include "Angle2D.h"

namespace Atmos::Render::Vulkan
{
    struct TexturedVertex
    {
        alignas(16) glm::vec4 color;
        alignas(8) glm::vec2 position;
        alignas(8) glm::vec2 texture;
    };

    using TexturedIndex = uint16_t;

    struct Textured
    {
        using Vertices = std::vector<TexturedVertex>;
        Vertices vertices;
        using Indices = std::vector<TexturedIndex>;
        Indices indices;
        Textured(const Vertices& vertices, const Indices& indices);
    };

    [[nodiscard]] std::optional<Textured> ToTextured(
        Color color,
        const Spatial::Point2D& worldTopLeft,
        const Spatial::Point2D& textureTopLeft,
        const Spatial::Size2D& baseSize,
        const Spatial::Size2D& textureSize,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle2D& rotation,
        const Spatial::Point2D& rotationCenter,
        const std::vector<Spatial::Point2D>& clipTo);
}