#pragma once

#include "ArcaRelicIncludes.h"
#include "RasterImage.h"
#include "RasterLine.h"
#include "RasterRegion.h"
#include "RasterText.h"
#include "PreparedRaster.h"

namespace Atmos::Render::Raster
{
    class Staged final
    {
    public:
        template<class T>
        using Objects = std::vector<Prepared<T>>;
    public:
        Objects<Image> images;
        Objects<Text> texts;
        Objects<Line> lines;
        Objects<Region> regions;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Raster::Staged>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::Raster::Staged";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::Raster::Staged, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::Raster::Staged>;
    };
}