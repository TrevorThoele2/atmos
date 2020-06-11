#pragma once

#include "LineWidth.h"
#include "Position2D.h"
#include "MaterialAsset.h"
#include "Color.h"

namespace Atmos::Render
{
    struct LineRender
    {
        std::vector<Position2D> points;
        Position2D::Value z;
        const Asset::Material* material;

        LineWidth width;

        Color color;
    };
}