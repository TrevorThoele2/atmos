#pragma once

#include <Arca/Command.h>

#include "ScriptModule.h"
#include "Buffer.h"

namespace Atmos::Scripting
{
    struct Compile
    {
        Module module;
        std::vector<Module> sharedData;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Compile>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Scripting::Compile";
        using Result = Atmos::Buffer;
    };
}