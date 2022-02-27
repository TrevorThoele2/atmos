#pragma once

#include "LineWidth.h"
#include "Point2D.h"
#include "MaterialAsset.h"
#include "Color.h"
#include "SurfaceResource.h"

namespace Atmos::Render::Raster
{
    struct Line
    {
        std::vector<Spatial::Point2D> points;
        const Asset::Material* material;
        LineWidth width;
        Color color;
        Resource::Surface* surface;
    };
}