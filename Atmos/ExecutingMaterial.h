#pragma once

#include "ArcaRelicIncludes.h"
#include "RasterImage.h"
#include "RasterLine.h"
#include "RasterRegion.h"
#include "RasterText.h"
#include "RasterCommand.h"

namespace Atmos::Render::Raster
{
    class ExecutingMaterial final
    {
    public:
        std::vector<Image> images;
        std::vector<Text> texts;
        std::vector<Line> lines;
        std::vector<Region> regions;

        std::vector<Command> recordedCommands = {};
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Raster::ExecutingMaterial>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::Raster::ExecutingMaterial";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::Raster::ExecutingMaterial, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::Raster::ExecutingMaterial>;
    };
}