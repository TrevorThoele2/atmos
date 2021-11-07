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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::ChangeColor";
    };
}