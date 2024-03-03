#pragma once

#include <Arca/Command.h>
#include "Position3D.h"

namespace Atmos::Render
{
    struct MoveCameraTo
    {
        Position3D toPosition;
        explicit MoveCameraTo(const Position3D& toPosition) : toPosition(toPosition)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MoveCameraTo>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "MoveCameraTo";
    };
}