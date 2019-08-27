#pragma once

#include <Arca/SignalTraits.h>
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
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}