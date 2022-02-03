#pragma once

#include <vector>
#include "Point2D.h"
#include "GridPoint.h"
#include "RenderMesh.h"

namespace Atmos::Render
{
    std::vector<Spatial::Point2D> Simplify(const std::vector<Spatial::Point2D>& points);

    std::vector<Spatial::Point2D> Triangulate(const std::vector<Spatial::Point2D>& points);
    std::vector<Spatial::Point2D> Triangulate(const std::vector<Spatial::Grid::Point>& points);
    
    std::vector<Spatial::Point2D> Clip(const std::vector<Spatial::Point2D>& points, const std::vector<Spatial::Point2D>& to);

    Mesh ConvertToMesh(const std::vector<Spatial::Point2D>& vertices);
}