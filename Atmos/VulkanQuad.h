#pragma once

#include <array>
#include <glm/glm.hpp>
#include "Color.h"
#include "Point2D.h"
#include "Size2D.h"
#include "Angle2D.h"
#include "AxisAlignedBox2D.h"

namespace Atmos::Render::Vulkan
{
    struct QuadVertex
    {
        alignas(16) glm::vec4 color;
        alignas(8) glm::vec2 position;
        alignas(8) glm::vec2 texture;
    };

    struct Quad
    {
        using Vertices = std::array<QuadVertex, 4>;
        Vertices vertices;
        Quad(const Vertices& vertices);
    };

    using QuadIndex = uint16_t;
    using QuadIndices = std::array<QuadIndex, 6>;
    static inline const QuadIndices quadIndices = { 0, 1, 2, 2, 1, 3 };
    static inline constexpr QuadIndex quadIndexIncrement = 4;

    [[nodiscard]] std::array<QuadVertex, 4> ToQuadVertices(
        Color color,
        Spatial::Point2D center,
        Spatial::Size2D size,
        Spatial::Angle2D rotation,
        Spatial::Point2D rotationCenter,
        Spatial::AxisAlignedBox2D texture);
}