#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Point3D.h"

namespace Atmos::Spatial
{
    struct MoveBounds
    {
        Arca::RelicID id = 0;
        Point3D to;

        MoveBounds(Arca::RelicID id, const Point3D& to) : id(id), to(to)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::MoveBounds>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Spatial::MoveBounds";
    };
}