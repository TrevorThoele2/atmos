#pragma once

#include <Arca/Signal.h>

#include "Scalers2D.h"

namespace Atmos::Spatial
{
    class BoundsScaled
    {
    public:
        Arca::RelicID id;
        Scalers2D previousScalers;
        Scalers2D newScalers;

        BoundsScaled() = default;
        explicit BoundsScaled(Arca::RelicID id, Scalers2D previousScalers, Scalers2D newScalers) :
            id(id), previousScalers(previousScalers), newScalers(newScalers)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::BoundsScaled>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Spatial::BoundsScaled"; }
    };
}