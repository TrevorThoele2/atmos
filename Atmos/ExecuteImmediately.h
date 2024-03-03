#pragma once

#include <Arca/Signal.h>
#include "RunningScript.h"

namespace Atmos::Script
{
    struct ExecuteImmediately
    {
        Arca::RelicIndex<RunningScript> script;
        explicit ExecuteImmediately(Arca::RelicIndex<RunningScript> script);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::ExecuteImmediately>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "ScriptExecuteImmediately";
    };
}