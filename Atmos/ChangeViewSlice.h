#pragma once

#include <Arca/Command.h>

#include <Arca/RelicID.h>
#include "AxisAlignedBox2D.h"

namespace Atmos::Render
{
    struct ChangeViewSlice
    {
        Arca::RelicID id = 0;

        Spatial::AxisAlignedBox2D to;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ChangeViewSlice>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Render::ChangeViewSlice"; }
    };
}