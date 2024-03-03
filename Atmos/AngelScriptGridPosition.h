#pragma once

#include "GridPoint.h"

class asIScriptEngine;

namespace Atmos::Script::Angel
{
    class GridPosition
    {
    public:
        Atmos::Spatial::Grid::Point::Value x, y, z;
    public:
        GridPosition();

        static void RegisterTo(asIScriptEngine* engine);
    };
}