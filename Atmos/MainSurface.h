#pragma once

#include "Surface.h"
#include "GraphicsManager.h"

namespace Atmos::Render
{
    class MainSurface final : public Surface<MainSurface>
    {
    public:
        MainSurface(Init init, GraphicsManager& manager, void* window);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MainSurface>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::MainSurface";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::MainSurface, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::MainSurface>;
    };
}