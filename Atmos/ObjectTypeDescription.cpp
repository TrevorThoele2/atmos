#include "ObjectTypeDescription.h"

#include <Inscription/VectorScribe.h>

namespace Atmos
{
    ObjectTypeDescription::ObjectTypeDescription()
    {}

    ObjectTypeDescription::ObjectTypeDescription(const ObjectTypeName& typeName, const BaseTypeList& baseTypes) : typeName(typeName), baseTypes(baseTypes)
    {}

    bool ObjectTypeDescription::operator==(const ObjectTypeDescription& arg) const
    {
        return typeName == arg.typeName;
    }

    bool ObjectTypeDescription::operator!=(const ObjectTypeDescription& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::ObjectTypeDescription, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.typeName);
        archive(object.baseTypes);
    }
}