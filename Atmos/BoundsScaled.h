#pragma once

#include <Arca/Signal.h>

#include "Bounds.h"

namespace Atmos::Spatial
{
    class BoundsScaled
    {
    public:
        Arca::Index<Bounds> bounds;

        BoundsScaled() = default;
        explicit BoundsScaled(Arca::Index<Bounds> bounds) : bounds(std::move(bounds))
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::BoundsScaled>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "Atmos::Spatial::BoundsScaled";
    };
}