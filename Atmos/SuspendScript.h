#pragma once

#include <Arca/Command.h>
#include "Script.h"

namespace Atmos::Scripting
{
    struct Suspend
    {
        Arca::Index<Script> script;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Suspend>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Scripting::Suspend"; }
    };
}