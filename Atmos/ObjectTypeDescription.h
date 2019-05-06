#pragma once

#include <vector>

#include "ObjectTypeName.h"

#include "Serialization.h"

namespace Atmos
{
    class ObjectTypeDescription
    {
    public:
        typedef std::vector<ObjectTypeName> BaseTypeList;
    public:
        const ObjectTypeName typeName;
        const BaseTypeList baseTypes;

        ObjectTypeDescription();
        ObjectTypeDescription(const ObjectTypeName& typeName, const BaseTypeList& baseTypes = BaseTypeList());
        ObjectTypeDescription(const ::Inscription::Table<ObjectTypeDescription>& table);

        bool operator==(const ObjectTypeDescription& arg) const;
        bool operator!=(const ObjectTypeDescription& arg) const;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}