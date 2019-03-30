
#include "Field.h"

namespace Atmos
{
    Field::Field(ID id, ObjectManager&& objectManager) : id(id), objectManager(std::move(objectManager))
    {}

    Field::Field(Field &&arg) : id(arg.id), objectManager(std::move(arg.objectManager))
    {}

    Field& Field::operator=(Field &&arg)
    {
        id = std::move(arg.id);
        objectManager = std::move(arg.objectManager);
        return *this;
    }

    Field::ID Field::GetID() const
    {
        return id;
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Field)
    {
        scribe(objectManager);
    }
}