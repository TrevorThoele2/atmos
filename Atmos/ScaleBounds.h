#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Scalers2D.h"

namespace Atmos::Spatial
{
    struct ScaleBounds
    {
        Arca::RelicID id = 0;
        Scalers2D to;

        ScaleBounds(Arca::RelicID id, const Scalers2D& to) : id(id), to(to)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::ScaleBounds>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Spatial::ScaleBounds"; }
    };
}