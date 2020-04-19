#pragma once

#include "Surface.h"
#include <Chroma/VariadicTemplate.h>

namespace Atmos::Render
{
    class GraphicsManager;
    class SurfaceCurator;

    class MainSurface final : public Surface<MainSurface>
    {
    public:
        MainSurface(Init init);
        MainSurface(MainSurface&& arg) noexcept = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MainSurface>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "MainRenderSurface";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::MainSurface, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Render::MainSurface, BinaryArchive>
    {};
}