#pragma once

#include <Arca/Command.h>

#include <Arca/RelicID.h>
#include "Color.h"

namespace Atmos::Render
{
    struct ChangeColor
    {
        Arca::RelicID id = 0;

        Color to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeColor>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::ChangeColor"; }
    };
}