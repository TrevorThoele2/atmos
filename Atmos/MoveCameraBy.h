#pragma once

#include <Arca/Signal.h>
#include "Position3D.h"

namespace Atmos::Render
{
    struct MoveCameraBy
    {
        Position3D::Value x;
        Position3D::Value y;
        Position3D::Value z;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MoveCameraBy>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "MoveCameraBy";
    };
}