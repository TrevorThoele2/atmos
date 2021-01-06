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
        static TypeName TypeName() { return "Atmos::Work"; }
    };
}