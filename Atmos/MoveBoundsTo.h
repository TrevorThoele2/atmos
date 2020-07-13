#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Point3D.h"

namespace Atmos::Spatial
{
    struct MoveBoundsTo
    {
        Arca::RelicID id = 0;
        Point3D to;

        MoveBoundsTo(Arca::RelicID id, const Point3D& to) : id(id), to(to)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::MoveBoundsTo>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Spatial::MoveBoundsTo";
    };
}