#pragma once

#include <Arca/Curator.h>

#include "ChangeSurfaceBackgroundColor.h"
#include "CreateMainSurfaceData.h"

namespace Atmos::Render
{
    class SurfaceCurator : public Arca::Curator
    {
    public:
        explicit SurfaceCurator(Init init);

        void Handle(const ChangeSurfaceBackgroundColor& command);
        void Handle(const CreateMainSurfaceData& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::SurfaceCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "SurfaceCurator";
        using HandledCommands = HandledCommands<
            Atmos::Render::ChangeSurfaceBackgroundColor,
            Atmos::Render::CreateMainSurfaceData>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::SurfaceCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Render::SurfaceCurator, BinaryArchive>
    {};
}