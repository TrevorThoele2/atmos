#pragma once

#include "Surface.h"
#include "GraphicsManager.h"

namespace Atmos::Render
{
    class MainSurface final : public Surface
    {
    public:
        MainSurface(Arca::RelicInit init, GraphicsManager& manager, void* window);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MainSurface>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::MainSurface";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::MainSurface, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::MainSurface>;
    };
}