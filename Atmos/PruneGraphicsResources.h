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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::PruneGraphicsResources";
    };
}