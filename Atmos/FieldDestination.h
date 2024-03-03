
#pragma once

#include "FieldID.h"
#include "GridPosition.h"
#include "Direction.h"

#include "Serialization.h"

namespace Atmos
{
    class FieldDestination
    {
    public:
        Direction direction;
        GridPosition position;
        FieldID id;
    public:
        FieldDestination() = default;
        FieldDestination(const Direction& direction, const GridPosition& position, FieldID id);
        FieldDestination(const FieldDestination& arg) = default;

        FieldDestination& operator=(const FieldDestination& arg) = default;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::FieldDestination, BinaryArchive> :
        public CompositeScribe<::Atmos::FieldDestination, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}