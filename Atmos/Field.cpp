#include "Field.h"

namespace Atmos::World
{
    Field::Field(FieldID id) : id(id)
    {}

    Field::Field(FieldID id, Arca::Reliquary& reliquary) : id(id), reliquary(&reliquary)
    {}

    Field& Field::operator=(Field&& arg) noexcept
    {
        id = arg.id;
        reliquary = arg.reliquary;
        return *this;
    }
}

namespace Inscription
{
    void Scribe<Atmos::World::Field, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(*object.reliquary);
    }
}