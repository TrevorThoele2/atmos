#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Point3D.h"

namespace Atmos::Spatial
{
    struct MoveBoundsBy
    {
        Arca::RelicID id = 0;
        Point3D delta;

        MoveBoundsBy(Arca::RelicID id, const Point3D& delta) : id(id), delta(delta)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::MoveBoundsBy>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Spatial::MoveBoundsBy";
    };
}