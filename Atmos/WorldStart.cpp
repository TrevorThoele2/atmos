
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

    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(WorldStart)
    {
        scribe(fieldID);
    }
}