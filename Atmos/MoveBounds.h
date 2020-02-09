#pragma once

#include <Arca/Command.h>
#include <Arca/RelicID.h>

#include "Position3D.h"

namespace Atmos
{
    struct MoveBounds
    {
        Arca::RelicID id = 0;
        Position3D toPosition;

        MoveBounds(Arca::RelicID id, const Position3D& toPosition) : id(id), toPosition(toPosition)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::MoveBounds>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "MoveBounds";
    };
}