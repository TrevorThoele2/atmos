
#pragma once

#include <vector>

#include "FieldID.h"
#include "ObjectManager.h"

#include "Serialization.h"

namespace Atmos
{
    class Field
    {
    public:
        typedef FieldID ID;
    public:
        ObjectManager objectManager;
    public:
        Field(ID id, ObjectManager&& objectManager);
        Field(Field &&arg);
        Field& operator=(Field &&arg);

        ID GetID() const;
    private:
        ID id;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}