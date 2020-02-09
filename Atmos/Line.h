#pragma once

#include "Position2D.h"
#include "Color.h"

namespace Atmos::Render
{
    struct Line
    {
        Position2D from;
        Position2D to;

        using Width = float;
        Width width;

        Color color;
    };
}