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

        bool operator==(const ObjectTypeDescription& arg) const;
        bool operator!=(const ObjectTypeDescription& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::ObjectTypeDescription, BinaryArchive> : public CompositeScribe<::Atmos::ObjectTypeDescription, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}