#pragma once

#include <Arca/Command.h>

namespace Atmos::Render::Resource
{
    struct SetupMainSurface
    {
        void* window;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Resource::SetupMainSurface>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "SetupMainSurfaceData";
    };
}