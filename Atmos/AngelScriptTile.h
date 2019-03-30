#pragma once

#include "AngelScriptObject.h"
#include "AngelScriptGridPosition.h"

namespace Atmos
{
    namespace Scripting
    {
        class Tile : public Object
        {
        public:
            GridPosition position;

            static Tile* Factory();

            static void RegisterToAngelScript(asIScriptEngine* engine);
        };
    }
}