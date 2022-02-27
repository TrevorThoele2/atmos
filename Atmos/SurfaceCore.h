#pragma once

#include "ArcaShardIncludes.h"
#include "SurfaceResource.h"
#include "Color.h"

namespace Atmos::Render
{
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
        static constexpr ObjectType objectType = ObjectType::Shard;
        static const inline TypeName typeName = "Atmos::Render::SurfaceCore";
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::SurfaceCore, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::SurfaceCore>;
    };
}