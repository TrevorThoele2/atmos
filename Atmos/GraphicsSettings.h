#pragma once

#include <Arca/ClosedTypedRelic.h>

namespace Atmos::Render
{
    class GraphicsSettings final : public Arca::ClosedTypedRelic<GraphicsSettings>
    {
    public:
        bool verticalSync = false;

        using ClosedTypedRelic::ClosedTypedRelic;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GraphicsSettings>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Render::GraphicsSettings";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::GraphicsSettings, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::GraphicsSettings>;
    };
}