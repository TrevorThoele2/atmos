#pragma once

#include <unordered_set>
#include "ArcaRelicIncludes.h"

#include "RenderCore.h"

#include "GridPoint.h"

#include <Inscription/UnorderedSetScribe.h>

namespace Atmos::Render
{
    class GridRegion final
    {
    public:
        Arca::Index<RenderCore> renderCore;

        std::unordered_set<Spatial::Grid::Point> points;
        Spatial::Grid::Point::Value z = 0;
    public:
        GridRegion(Arca::RelicInit init);
        GridRegion(
            Arca::RelicInit init,
            const std::unordered_set<Spatial::Grid::Point>& points,
            Spatial::Grid::Point::Value z,
            Arca::Index<Asset::Material> material);

        bool operator==(const GridRegion& arg) const;
        bool operator!=(const GridRegion& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GridRegion>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Render::GridRegion";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::GridRegion> final
    {
    public:
        using ObjectT = Atmos::Render::GridRegion;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("renderCore", object.renderCore);
            format("points", object.points);
            format("z", object.z);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::GridRegion, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::GridRegion>;
    };
}