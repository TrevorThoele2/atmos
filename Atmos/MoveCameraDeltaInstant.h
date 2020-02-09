#pragma once

#include <Arca/Command.h>
#include "Position3D.h"

namespace Atmos::Render
{
    struct MoveCameraDeltaInstant
    {
        Position3D::Value x;
        Position3D::Value y;
        Position3D::Value z;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MoveCameraDeltaInstant>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "MoveCameraDeltaInstant";
    };
}