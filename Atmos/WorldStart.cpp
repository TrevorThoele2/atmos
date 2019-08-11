#include "WorldStart.h"

namespace Atmos::World
{
    WorldStart::WorldStart(FieldID fieldID) : fieldID(fieldID)
    {}

    WorldStart::WorldStart(const WorldStart& arg) : fieldID(arg.fieldID)
    {}

    WorldStart& WorldStart::operator=(const WorldStart& arg)
    {
        fieldID = arg.fieldID;
        return *this;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::World::WorldStart, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.fieldID);
    }

    void Scribe<::Atmos::World::WorldStart, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}