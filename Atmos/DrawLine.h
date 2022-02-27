#pragma once

#include "LineWidth.h"
#include "Point2D.h"
#include "Color.h"
#include "Shaders.h"
#include "SurfaceResource.h"

namespace Atmos::Render::Raster
{
    struct DrawLine
    {
        Shaders shaders;
        const Resource::Surface* surface = nullptr;
        std::vector<Spatial::Point2D> points;
        LineWidth width;
        Color color;
    };
}