#include "CurrentFieldData.h"

namespace Atmos::World
{
    CurrentFieldData::CurrentFieldData(Init init)
        : ClosedTypedRelic(init)
    {}

    CurrentFieldData::CurrentFieldData(Init init, FieldID fieldID)
        : ClosedTypedRelic(init), fieldID(fieldID)
    {}
}