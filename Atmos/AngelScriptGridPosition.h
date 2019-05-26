#pragma once

#include "GridPosition.h"

class asIScriptEngine;

namespace Atmos
{
    class ObjectManager;

    namespace Scripting
    {
        class GridPosition
        {
        public:
            ::Atmos::GridPosition::Value x, y, z;
        public:
            GridPosition();

            static void RegisterToAngelScript(asIScriptEngine* engine);
        };
    }
}