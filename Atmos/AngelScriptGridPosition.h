#pragma once

#include "GridPosition.h"

class asIScriptEngine;

namespace Atmos
{
    class ObjectManager;
}

namespace Atmos::Script
{
    class GridPosition
    {
    public:
        ::Atmos::Grid::Position::Value x, y, z;
    public:
        GridPosition();

        static void RegisterToAngelScript(asIScriptEngine* engine);
    };
}