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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Work";
    };
}