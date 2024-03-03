#pragma once

#include <Arca/Curator.h>

#include "ChangeSurfaceBackgroundColor.h"

namespace Atmos::Render
{
    class SurfaceCurator : public Arca::Curator
    {
    public:
        void Handle(const ChangeSurfaceBackgroundColor& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::SurfaceCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "SurfaceCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::SurfaceCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Render::SurfaceCurator, BinaryArchive>
    {};
}