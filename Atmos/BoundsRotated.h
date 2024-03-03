#pragma once

#include <Arca/Signal.h>

#include "Bounds.h"

namespace Atmos::Spatial
{
    class BoundsRotated
    {
    public:
        Arca::Index<Bounds> bounds;

        BoundsRotated() = default;
        explicit BoundsRotated(Arca::Index<Bounds> bounds) : bounds(std::move(bounds))
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::BoundsRotated>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "Atmos::Spatial::BoundsRotated";
    };
}