#pragma once

#include "Position2D.h"
#include "Color.h"

namespace Atmos::Render
{
    class Line
    {
    public:
        Position2D from;
        Position2D to;

        using Width = float;
        Width width;

        Color color;
    };
}