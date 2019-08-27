#pragma once

#include "GridPosition.h"

class asIScriptEngine;

namespace Atmos::Script::Angel
{
    class GridPosition
    {
    public:
        ::Atmos::Grid::Position::Value x, y, z;
    public:
        GridPosition();

        static void RegisterTo(asIScriptEngine* engine);
    };
}