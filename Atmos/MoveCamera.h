#pragma once

#include <Arca/Signal.h>
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
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}