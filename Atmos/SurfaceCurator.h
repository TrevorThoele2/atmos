#pragma once

#include <Arca/Curator.h>

#include "ChangeSurfaceBackgroundColor.h"

namespace Atmos::Render
{
    class SurfaceCurator : public Arca::Curator
    {
    public:
        explicit SurfaceCurator(Init init);

        void Handle(const ChangeMainSurfaceBackgroundColor& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::SurfaceCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Render::SurfaceCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Render::ChangeMainSurfaceBackgroundColor>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::SurfaceCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::SurfaceCurator>;
    };
}