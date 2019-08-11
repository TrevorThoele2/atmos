#include "Field.h"

namespace Atmos::World
{
    Field::Field(ID id) : _id(id),
        id([this]() { return _id; })
    {}

    Field::Field(Field&& arg) : _id(std::move(arg._id)),
        id([this]() { return _id; })
    {}

    Field& Field::operator=(Field&& arg)
    {
        _id = std::move(arg._id);
        objectManager = std::move(arg.objectManager);
        return *this;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::World::Field, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.objectManager);
    }

    void Scribe<::Atmos::World::Field, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}