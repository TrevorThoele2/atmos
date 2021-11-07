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
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Render::Resource::CreateSurface";
        using Result = std::unique_ptr<Atmos::Render::Resource::Surface>;
    };
}