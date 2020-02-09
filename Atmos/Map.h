#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "SparseRegion.h"

namespace Atmos::World
{
    class Map final : public Arca::ClosedTypedRelic<Map>
    {
    public:
        Grid::SparseRegion definedRegion;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::Map>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Map";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::World::Map, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::World::Map, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}