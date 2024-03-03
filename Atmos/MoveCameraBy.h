#pragma once

#include <Arca/SignalTraits.h>
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
        static const TypeName typeName;
    };
}