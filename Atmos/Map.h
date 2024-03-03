#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "SparseRegion.h"

namespace Atmos::World
{
    class Map final : public Arca::ClosedTypedRelicAutomation<Map>
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
        static const TypeName typeName;
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