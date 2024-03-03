
#pragma once

#include "FieldID.h"

#include "StoredProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class WorldStart
    {
    public:
        StoredProperty<FieldID> fieldID;
    public:
        WorldStart(FieldID fieldID = 0);
        WorldStart(const WorldStart& arg);
        WorldStart& operator=(const WorldStart& arg);
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}