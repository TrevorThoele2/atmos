#include "WorldStart.h"

namespace Atmos
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
    void Scribe<::Atmos::WorldStart, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.fieldID);
    }
}