#pragma once

#include <Arca/Command.h>
#include "Position2D.h"

namespace Atmos::Render
{
    struct MoveCameraBy
    {
        Position2D::Value x;
        Position2D::Value y;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MoveCameraBy>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "MoveCameraBy";
    };
}