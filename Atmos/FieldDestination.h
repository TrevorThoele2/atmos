#pragma once

#include "FieldID.h"
#include "GridPoint.h"
#include "Angle2D.h"

#include "Serialization.h"

namespace Atmos::World
{
    struct FieldDestination
    {
        Spatial::Angle2D direction;
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