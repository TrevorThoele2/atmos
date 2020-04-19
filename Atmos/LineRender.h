#pragma once

#include "LineWidth.h"
#include "Position2D.h"
#include "Color.h"

namespace Atmos::Render
{
    struct LineRender
    {
        std::vector<Position2D> points;
        Position2D::Value z;

        LineWidth width;

        Color color;
    };
}