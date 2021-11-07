#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Color.h"

namespace Atmos::Render
{
    struct ChangeMainSurfaceBackgroundColor
    {
        Arca::RelicID id;
        Color color;
        ChangeMainSurfaceBackgroundColor(Arca::RelicID id, Color color);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeMainSurfaceBackgroundColor>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::ChangeSurfaceBackgroundColor";
    };
}