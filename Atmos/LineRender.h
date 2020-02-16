#pragma once

#include "LineWidth.h"
#include "Position2D.h"
#include "Color.h"

namespace Atmos::Render
{
    struct LineRender
    {
        Position2D from;
        Position2D to;
        Position2D::Value z;

        LineWidth width;

        Color color;
    };
}