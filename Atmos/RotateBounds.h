#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Angle2D.h"

namespace Atmos::Spatial
{
    struct RotateBounds
    {
        Arca::RelicID id = 0;
        Angle2D to;

        RotateBounds(Arca::RelicID id, const Angle2D& to) : id(id), to(to)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::RotateBounds>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Spatial::RotateBounds"; }
    };
}