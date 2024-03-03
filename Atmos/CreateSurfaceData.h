#pragma once

#include <Arca/Command.h>
#include "SurfaceData.h"

namespace Atmos::Render
{
    struct CreateSurfaceData
    {
        void* window;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CreateSurfaceData>
    {
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "CreateSurfaceData";
        using Result = std::unique_ptr<Atmos::Render::SurfaceData>;
    };
}