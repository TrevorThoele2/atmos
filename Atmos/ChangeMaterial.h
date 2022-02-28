#pragma once

#include <Arca/Command.h>

#include "Material.h"

namespace Atmos::Render
{
    struct ChangeMaterial
    {
        Arca::RelicID id = 0;

        Material to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeMaterial>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::ChangeMaterial";
    };
}