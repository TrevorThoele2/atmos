
#pragma once

#include <vector>

#include "FieldID.h"
#include "ObjectManager.h"

#include "ReadonlyProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class Field
    {
    public:
        typedef FieldID ID;
        ReadonlyProperty<ID> id;
    public:
        ObjectManager objectManager;
    public:
        Field(ID id);
        Field(Field&& arg);
        Field& operator=(Field&& arg);
    private:
        ID _id;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}