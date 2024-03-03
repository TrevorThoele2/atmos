#pragma once

#include "ArcaRelicIncludes.h"
#include "StaticImage.h"
#include "SparseRegion.h"

#include <Inscription/UnorderedSetScribe.h>

namespace Atmos::World
{
    class Map final
    {
    public:
        using Terrain = std::vector<Arca::Index<Render::StaticImage>>;
        Terrain terrain;

        Spatial::Grid::SparseRegion entityBoundary;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::Map>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::World::Map";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::World::Map> final
    {
    public:
        using ObjectT = Atmos::World::Map;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("terrain", object.terrain);
            archive("entityBoundary", object.entityBoundary);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::World::Map, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::World::Map>;
    };
}