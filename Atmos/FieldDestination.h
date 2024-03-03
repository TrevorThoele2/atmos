#pragma once

#include "FieldID.h"
#include "GridPoint.h"
#include "Direction.h"

#include "Serialization.h"

namespace Atmos::World
{
    struct FieldDestination
    {
        Spatial::Direction direction;
        Spatial::Grid::Point position;
        FieldID id;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::World::FieldDestination, BinaryArchive> final :
        public CompositeScribe<::Atmos::World::FieldDestination, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}