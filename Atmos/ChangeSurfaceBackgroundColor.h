#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Color.h"

namespace Atmos::Render
{
    struct ChangeSurfaceBackgroundColor
    {
        Arca::RelicID id;
        Color color;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeSurfaceBackgroundColor>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline TypeName typeName = "ChangeSurfaceBackgroundColor";
    };
}