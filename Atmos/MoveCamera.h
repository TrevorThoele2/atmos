#pragma once

#include <Arca/Command.h>
#include "Direction.h"
#include "Position3D.h"

namespace Atmos::Render
{
    struct MoveCamera
    {
        Direction direction;
        Position3D::Value moveBy;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MoveCamera>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "MoveCamera";
    };
}