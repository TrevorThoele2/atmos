#pragma once

#include <Arca/Command.h>

namespace Atmos::Render
{
    struct PruneGraphicsResources
    {};
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::PruneGraphicsResources>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::PruneGraphicsResources"; }
    };
}