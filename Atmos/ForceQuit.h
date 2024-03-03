#pragma once

#include <Arca/Signal.h>
#include "RunningScript.h"

namespace Atmos::Script
{
    struct ForceQuit
    {
        Arca::LocalPtr<RunningScript> script;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::ForceQuit>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "ScriptForceQuit";
    };
}