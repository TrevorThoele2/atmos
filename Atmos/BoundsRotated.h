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
        Angle2D newRotation;

        BoundsRotated() = default;
        explicit BoundsRotated(Arca::RelicID id, Angle2D previousRotation, Angle2D newRotation) :
            id(id), previousRotation(previousRotation), newRotation(newRotation)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::BoundsRotated>
    {
        static constexpr ObjectType objectType = ObjectType::Signal;
        static const inline TypeName typeName = "Atmos::Spatial::BoundsRotated";
    };
}