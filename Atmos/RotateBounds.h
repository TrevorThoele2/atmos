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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Spatial::RotateBounds";
    };
}