#include "Field.h"

namespace Atmos::World
{
    Field::Field(FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary) :
        id(id), reliquary(std::move(reliquary))
    {}

    Field::Field(Field&& arg) noexcept : id(arg.id), reliquary(std::move(arg.reliquary))
    {}

    Field& Field::operator=(Field&& arg) noexcept
    {
        id = arg.id;
        reliquary = std::move(arg.reliquary);
        return *this;
    }

    FieldID Field::ID() const
    {
        return id;
    }

    Arca::Reliquary& Field::Reliquary()
    {
        return *reliquary;
    }

    const Arca::Reliquary& Field::Reliquary() const
    {
        return *reliquary;
    }
}

namespace Inscription
{
    void Scribe<Atmos::World::Field, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(*object.reliquary);
    }
}