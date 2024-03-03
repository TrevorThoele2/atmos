#pragma once

#include <Arca/Signal.h>

#include "Point3D.h"

namespace Atmos::Spatial
{
    class BoundsMoved
    {
    public:
        Arca::RelicID id;
        Point3D previousPosition;
        Point3D newPosition;

        BoundsMoved() = default;
        explicit BoundsMoved(Arca::RelicID id, Point3D previousPosition, Point3D newPosition) :
            id(id), previousPosition(previousPosition), newPosition(newPosition)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::BoundsMoved>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Spatial::BoundsMoved"; }
    };
}