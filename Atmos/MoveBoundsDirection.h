#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Angle3D.h"
#include "Point3D.h"

namespace Atmos::Spatial
{
    struct MoveBoundsDirection
    {
        Arca::RelicID id = 0;
        Angle3D direction;
        Point3D::Value amount;

        MoveBoundsDirection(Arca::RelicID id, Angle3D direction, Point3D::Value amount) :
            id(id), direction(direction), amount(amount)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::MoveBoundsDirection>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Spatial::MoveBoundsDirection";
    };
}