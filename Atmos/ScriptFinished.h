#pragma once

#include <Arca/Signal.h>

#include "Script.h"
#include "ScriptResult.h"

namespace Atmos::Scripting
{
    struct Finished
    {
        Arca::Index<Script> script;

        Result result;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Finished>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Scripting::Finished"; }
    };
}