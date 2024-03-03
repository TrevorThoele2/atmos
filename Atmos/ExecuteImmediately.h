#pragma once

#include <Arca/Signal.h>
#include "RunningScript.h"

namespace Atmos::Script
{
    struct ExecuteImmediately
    {
        RunningScript& script;
        explicit ExecuteImmediately(RunningScript& script);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Script::ExecuteImmediately>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}