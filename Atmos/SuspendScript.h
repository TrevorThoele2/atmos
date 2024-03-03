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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Scripting::Suspend";
    };
}