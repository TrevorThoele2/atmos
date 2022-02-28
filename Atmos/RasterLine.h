#pragma once

#include "LineWidth.h"
#include "Point2D.h"
#include "Material.h"
#include "Color.h"

namespace Atmos::Render::Raster
{
    struct Line
    {
        std::vector<Spatial::Point2D> points;
        Material material;
        LineWidth width;
        Color color;
    };
}