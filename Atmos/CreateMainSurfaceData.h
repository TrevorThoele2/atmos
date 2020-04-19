#pragma once

#include <Arca/Command.h>

namespace Atmos::Render
{
    struct CreateMainSurfaceData
    {
        void* window;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CreateMainSurfaceData>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "CreateMainSurfaceData";
    };
}