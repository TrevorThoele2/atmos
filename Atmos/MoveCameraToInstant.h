#pragma once

#include <Arca/Command.h>
#include "Position3D.h"

namespace Atmos::Render
{
    struct MoveCameraToInstant
    {
        Position3D toPosition;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MoveCameraToInstant>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "MoveCameraToInstant";
    };
}