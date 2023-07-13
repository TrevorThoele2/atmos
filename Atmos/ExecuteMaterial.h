#pragma once

#include <Arca/Command.h>
#include "ScriptData.h"
#include "ScriptResult.h"

namespace Atmos::Scripting
{
    struct ExecuteMaterial
    {
        ScriptData* data;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::ExecuteMaterial>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Scripting::ExecuteMaterial";
        using Result = std::optional<Atmos::Scripting::Result>;
    };
}