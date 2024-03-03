#pragma once

#include <vector>

#include "Point2D.h"

namespace Atmos::Render
{
    struct Mesh
    {
        std::vector<Spatial::Point2D> vertices;
        std::vector<std::uint16_t> indices;
    };
}