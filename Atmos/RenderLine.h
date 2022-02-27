#pragma once

#include "LineWidth.h"
#include "Point2D.h"
#include "MaterialAsset.h"
#include "Color.h"
#include "SurfaceResource.h"

namespace Atmos::Render
{
    struct RenderLine
    {
        std::vector<Spatial::Point2D> points;
        Spatial::Point2D::Value z;

        const Asset::Material* material;

        LineWidth width;

        Color color;

        int space;

        Resource::Surface* surface;
    };
}