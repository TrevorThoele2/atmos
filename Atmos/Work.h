#pragma once

#include <Arca/Command.h>

namespace Atmos
{
    struct Work
    {};
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Work>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Work";
    };
}