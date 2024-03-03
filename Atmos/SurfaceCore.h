#pragma once

#include <Arca/Shard.h>
#include "SurfaceData.h"
#include <memory>

namespace Atmos::Render
{
    struct SurfaceCore
    {
        using DataT = SurfaceData;
        using DataPtr = std::unique_ptr<DataT>;
        DataPtr data;

        SurfaceCore() = default;
        explicit SurfaceCore(DataPtr&& data) : data(std::move(data))
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::SurfaceCore>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "RenderSurfaceCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::SurfaceCore, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Render::SurfaceCore, BinaryArchive>
    {};
}