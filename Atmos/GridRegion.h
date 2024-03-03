#pragma once

#include <unordered_set>
#include <Arca/Relic.h>

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
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GridRegion>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Render::GridRegion"; }
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
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("renderCore", object.renderCore);
            archive("points", object.points);
            archive("z", object.z);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::GridRegion, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::GridRegion>;
    };
}