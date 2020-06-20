#pragma once

#include <Arca/Signal.h>
#include "RunningScript.h"

namespace Atmos::Script
{
    struct ExecuteImmediately
    {
        Arca::Index<RunningScript> script;
        explicit ExecuteImmediately(Arca::Index<RunningScript> script);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::ExecuteImmediately>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Script::ExecuteImmediately";
    };
}