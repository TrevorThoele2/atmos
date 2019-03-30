
#include "ObjectTypeDescription.h"

#include <Inscription/Vector.h>

namespace Atmos
{
    ObjectTypeDescription::ObjectTypeDescription()
    {}

    ObjectTypeDescription::ObjectTypeDescription(const ObjectTypeName& typeName, const BaseTypeList& baseTypes) : typeName(typeName), baseTypes(baseTypes)
    {}

    ObjectTypeDescription::ObjectTypeDescription(const ::Inscription::Table<ObjectTypeDescription>& table) :
        INSCRIPTION_TABLE_GET_MEM(typeName), INSCRIPTION_TABLE_GET_MEM(baseTypes)
    {}

    bool ObjectTypeDescription::operator==(const ObjectTypeDescription& arg) const
    {
        return typeName == arg.typeName;
    }

    bool ObjectTypeDescription::operator!=(const ObjectTypeDescription& arg) const
    {
        return !(*this == arg);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ObjectTypeDescription)
    {
        scribe(::Inscription::RemoveConst(typeName));
        scribe(::Inscription::RemoveConst(baseTypes));
    }
}