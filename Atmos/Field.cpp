
#include "Field.h"

namespace Atmos
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

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Field)
    {
        scribe(objectManager);
    }
}