#pragma once

#include <Arca/Signal.h>

#include "Bounds.h"

namespace Atmos::Spatial
{
    class BoundsMoved
    {
    public:
        Arca::Index<Bounds> bounds;

        explicit BoundsMoved(Arca::Index<Bounds> bounds) : bounds(std::move(bounds))
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::BoundsMoved>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "Atmos::Spatial::BoundsMoved";
    };
}