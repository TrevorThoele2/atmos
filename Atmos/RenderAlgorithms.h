#pragma once

#include <vector>
#include "Position2D.h"
#include "GridPosition.h"
#include "RenderMesh.h"

namespace Atmos::Render
{
    std::vector<Position2D> Simplify(const std::vector<Position2D>& points);

    std::vector<Position2D> Triangulate(const std::vector<Position2D>& points);
    std::vector<Position2D> Triangulate(const std::vector<Grid::Position>& points);
    
    Mesh ConvertToMesh(const std::vector<Position2D>& vertices);
}