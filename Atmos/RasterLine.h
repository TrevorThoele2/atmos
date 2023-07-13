#pragma once

#include "LineWidth.h"
#include "Point2D.h"
#include "MaterialAsset.h"
#include "Color.h"

namespace Atmos::Render::Raster
{
    struct Line
    {
        Arca::Index<Asset::Material> material;
        std::vector<Spatial::Point2D> points;
        LineWidth width;
        Color color;
    };
}