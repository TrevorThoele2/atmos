#pragma once

#include "AngelScriptRelic.h"
#include "AngelScriptGridPosition.h"

namespace Atmos::Script::Angel
{
    class Tile : public Relic
    {
    public:
        GridPosition position;
    public:
        Tile();

        static void RegisterTo(asIScriptEngine* engine);
    };
}