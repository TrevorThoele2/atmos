#pragma once

#include <Arca/Command.h>
#include "Script.h"

namespace Atmos::Scripting
{
    struct ModifyData
    {
        Arca::Index<Script> script;
        std::vector<Datum> add;
        std::vector<String> remove;
        std::vector<Datum> replace;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::ModifyData>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Scripting::ModifyData";
    };
}