#pragma once

#include <vector>
#include "Point2D.h"
#include "GridPoint.h"
#include "RenderMesh.h"
#include "Space.h"

namespace Atmos::Render
{
    [[nodiscard]] int Ordering(Spatial::Space space);

    [[nodiscard]] std::vector<Spatial::Point2D> Simplify(const std::vector<Spatial::Point2D>& points);

    [[nodiscard]] std::vector<Spatial::Point2D> Triangulate(const std::vector<Spatial::Point2D>& points);
    [[nodiscard]] std::vector<Spatial::Point2D> Triangulate(const std::vector<Spatial::Grid::Point>& points);
    
    [[nodiscard]] Mesh ConvertToMesh(const std::vector<Spatial::Point2D>& vertices);
}