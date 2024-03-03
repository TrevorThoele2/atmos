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

        BoundsScaled() = default;
        explicit BoundsScaled(Arca::RelicID id, Scalers2D previousScalers) : id(id), previousScalers(previousScalers)
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