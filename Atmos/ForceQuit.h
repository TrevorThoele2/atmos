#pragma once

#include <Arca/Signal.h>
#include "RunningScript.h"

namespace Atmos::Script
{
    struct ForceQuit
    {
        Arca::Index<RunningScript> script;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::ForceQuit>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "ScriptForceQuit";
    };
}