#pragma once

#include <Arca/Command.h>

#include "ScriptModule.h"
#include "ScriptCompiledModule.h"

namespace Atmos::Scripting
{
    struct Compile
    {
        std::vector<Module> modules;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Compile>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Scripting::Compile";
        using Result = std::vector<Atmos::Scripting::CompiledModule>;
    };
}