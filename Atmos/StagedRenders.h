#pragma once

#include "ArcaRelicIncludes.h"
#include "RenderImage.h"
#include "RenderText.h"
#include "RenderLine.h"
#include "RenderRegion.h"

namespace Atmos::Render
{
    class StagedRenders final
    {
    public:
        std::vector<RenderImage> images;
        std::vector<RenderText> texts;
        std::vector<RenderLine> lines;
        std::vector<RenderRegion> regions;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::StagedRenders>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::StagedRenders";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::StagedRenders, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::StagedRenders>;
    };
}