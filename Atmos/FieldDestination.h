#pragma once

#include "FieldID.h"
#include "GridPosition.h"
#include "Direction.h"

#include "Serialization.h"

namespace Atmos::World
{
    class FieldDestination
    {
    public:
        Direction direction;
        Grid::Position position;
        FieldID id;
    public:
        FieldDestination() = default;
        FieldDestination(const Direction& direction, const Grid::Position& position, FieldID id);
        FieldDestination(const FieldDestination& arg) = default;

        FieldDestination& operator=(const FieldDestination& arg) = default;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::World::FieldDestination, BinaryArchive> :
        public CompositeScribe<::Atmos::World::FieldDestination, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}