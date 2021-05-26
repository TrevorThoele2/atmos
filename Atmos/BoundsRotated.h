#pragma once

#include <Arca/Signal.h>

#include "Angle2D.h"

namespace Atmos::Spatial
{
    class BoundsRotated
    {
    public:
        Arca::RelicID id;
        Angle2D previousRotation;

        BoundsRotated() = default;
        explicit BoundsRotated(Arca::RelicID id, Angle2D previousRotation) : id(id), previousRotation(previousRotation)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::BoundsRotated>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Spatial::BoundsRotated"; }
    };
}