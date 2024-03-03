#pragma once

#include "GridPosition.h"

class asIScriptEngine;

namespace Atmos
{
    namespace Scripting
    {
        class GridPosition
        {
        public:
            ::Atmos::GridPosition::ValueT x, y, z;

            static void Constructor(void* memory);
            static void Destructor(void* memory);

            static void RegisterToAngelScript(asIScriptEngine* engine);
        };
    }
}