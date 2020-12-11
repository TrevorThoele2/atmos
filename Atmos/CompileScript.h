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
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Scripting::Compile";
        using Result = Atmos::DataBuffer;
    };
}