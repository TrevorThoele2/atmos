#pragma once

#include <vector>

#include "Position2D.h"

namespace Atmos::Render
{
    struct Mesh
    {
        std::vector<Position2D> vertices;
        std::vector<std::uint16_t> indices;
    };
}