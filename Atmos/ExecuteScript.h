#pragma once

#include <Arca/Command.h>
#include "Script.h"
#include "ScriptResult.h"

namespace Atmos::Scripting
{
    struct Execute
    {
        Arca::Index<Script> script;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Execute>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Scripting::Execute";
        using Result = std::optional<Atmos::Scripting::Result>;
    };
}