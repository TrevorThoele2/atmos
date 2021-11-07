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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::ChangeViewSlice";
    };
}