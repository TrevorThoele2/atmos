#pragma once

#include <unordered_set>
#include <Arca/Relic.h>

#include "RegionMaterialAsset.h"
#include "GridPoint.h"

#include <Inscription/UnorderedSetScribe.h>

namespace Atmos::Render
{
    class GridRegion final
    {
    public:
        std::unordered_set<Spatial::Grid::Point> points;
        Spatial::Grid::Point::Value z;
        Arca::Index<Asset::RegionMaterial> material;
    public:
        GridRegion() = default;
        GridRegion(
            const std::unordered_set<Spatial::Grid::Point>& points,
            Spatial::Grid::Point::Value z,
            Arca::Index<Asset::RegionMaterial> material);
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
            archive("points", object.points);
            archive("z", object.z);
            archive("material", object.material);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::GridRegion, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::GridRegion>;
    };
}