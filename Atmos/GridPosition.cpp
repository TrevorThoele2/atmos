#include "GridPosition.h"

#include <Inscription/Scribe.h>

namespace Atmos::Grid
{
    Position::Position(Value x, Value y) : x(x), y(y)
    {}

    bool Position::operator==(const Position& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Position::operator!=(const Position& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Grid::Position, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
    }
}