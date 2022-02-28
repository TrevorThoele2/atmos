#pragma once

#include <array>
#include <glm/glm.hpp>
#include "Color.h"
#include "Point2D.h"
#include "Size2D.h"
#include "Angle2D.h"

namespace Atmos::Render::Vulkan
{
    static inline constexpr size_t verticesPerQuad = 4;
    static inline constexpr size_t indicesPerQuad = 6;

    struct QuadVertex
    {
        alignas(16) glm::vec4 color;
        alignas(8) glm::vec2 position;
        alignas(8) glm::vec2 texture;
    };

    struct Quad
    {
        using Vertices = std::array<QuadVertex, verticesPerQuad>;
        Vertices vertices;
    };

    using QuadIndex = uint16_t;
    using QuadIndices = std::array<QuadIndex, indicesPerQuad>;
    static inline const QuadIndices quadIndices = { 0, 1, 2, 2, 1, 3 };
    static inline constexpr QuadIndex quadIndexIncrement = 4;

    [[nodiscard]] Quad ToQuad(
        Color color,
        Spatial::Point2D center,
        Spatial::Size2D size,
        Spatial::Angle2D rotation,
        Spatial::Point2D rotationCenter,
        Spatial::AxisAlignedBox2D texture);
}