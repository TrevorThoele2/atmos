#pragma once

#include <Arca/SignalTraits.h>
#include "RunningScript.h"

namespace Atmos::Script
{
    struct ForceQuit
    {
        RunningScript& script;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::ForceQuit>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}