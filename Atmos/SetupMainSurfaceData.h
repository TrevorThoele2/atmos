#pragma once

#include <Arca/Command.h>
#include "SurfaceData.h"

namespace Atmos::Render
{
    struct SetupMainSurfaceData
    {
        void* window;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::SetupMainSurfaceData>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "SetupMainSurfaceData";
    };
}