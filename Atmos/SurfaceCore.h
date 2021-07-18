#pragma once

#include <Arca/Shard.h>
#include "SurfaceResource.h"

namespace Atmos::Render
{
    struct ImageRender;
    struct LineRender;
    struct RegionRender;

    struct SurfaceCore
    {
        using ResourceT = Resource::Surface;
        using ResourcePtr = std::unique_ptr<ResourceT>;
        ResourcePtr resource;

        Color backgroundColor;

        SurfaceCore() = default;
        SurfaceCore(ResourcePtr&& resource);

        [[nodiscard]] ResourceT* Resource();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::SurfaceCore>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static TypeName TypeName() { return "Atmos::Render::SurfaceCore"; }
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::SurfaceCore, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::SurfaceCore>;
    };
}