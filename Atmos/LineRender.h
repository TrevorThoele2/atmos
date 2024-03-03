#pragma once

#include "LineWidth.h"
#include "Point2D.h"
#include "MaterialAsset.h"
#include "Color.h"

namespace Atmos::Render
{
    struct LineRender
    {
        std::vector<Spatial::Point2D> points;
        Spatial::Point2D::Value z;

        Arca::RelicID materialID;
        const Asset::Material* material;

        LineWidth width;

        Color color;
    };
}