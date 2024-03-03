#pragma once

#include "AngelScriptObject.h"
#include "AngelScriptGridPosition.h"

namespace Atmos
{
    class ObjectManager;
}

namespace Atmos::Scripting
{
    class Tile : public Object
    {
    public:
        GridPosition position;
    public:
        Tile();

        static void RegisterToAngelScript(asIScriptEngine* engine);
    };
}