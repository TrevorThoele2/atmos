#pragma once

#include <Arca/Shard.h>
#include "SurfaceResource.h"
#include "Color.h"
#include <memory>

namespace Atmos::Render
{
    struct SurfaceCore
    {
        using ResourceT = Resource::Surface;
        using ResourcePtr = std::unique_ptr<ResourceT>;
        ResourcePtr resource;

        Color backgroundColor;

        SurfaceCore() = default;
        explicit SurfaceCore(ResourcePtr&& resource) : resource(std::move(resource))
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::SurfaceCore>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "Atmos::Render::SurfaceCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::SurfaceCore, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Render::SurfaceCore, BinaryArchive>
    {};
}