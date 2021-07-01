#pragma once

#include <Arca/Signal.h>

#include "Bounds.h"
#include <Arca/RelicID.h>

namespace Atmos::Spatial
{
    class BoundsChanged
    {
    public:
        Arca::RelicID id;
        Bounds previousBounds;
        Bounds newBounds;

        BoundsChanged() = default;
        explicit BoundsChanged(Arca::RelicID id, const Bounds& previousBounds, const Bounds& newBounds) :
            id(id), previousBounds(previousBounds), newBounds(newBounds)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::BoundsChanged>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Spatial::BoundsChanged"; }
    };
}