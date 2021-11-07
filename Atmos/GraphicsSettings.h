#pragma once

#include "ArcaRelicIncludes.h"

namespace Atmos::Render
{
    class GraphicsSettings final
    {
    public:
        bool verticalSync = false;

        GraphicsSettings() = default;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GraphicsSettings>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::GraphicsSettings";
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