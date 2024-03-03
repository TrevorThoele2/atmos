#pragma once

#include <Arca/Command.h>
#include "SurfaceResource.h"

namespace Atmos::Render::Resource
{
    struct CreateSurface
    {
        void* window;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Resource::CreateSurface>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "CreateSurfaceData";
        using Result = std::unique_ptr<Atmos::Render::Resource::Surface>;
    };
}