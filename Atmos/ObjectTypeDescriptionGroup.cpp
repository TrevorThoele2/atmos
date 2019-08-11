#include "ObjectTypeDescriptionGroup.h"

#include <Inscription/VectorScribe.h>

namespace Atmos
{
    ObjectTypeDescriptionGroup::ObjectTypeDescriptionGroup()
    {}

    ObjectTypeDescriptionGroup::ObjectTypeDescriptionGroup(const List& list) : list(list)
    {}

    ObjectTypeDescriptionGroup::ObjectTypeDescriptionGroup(List&& list) : list(std::move(list))
    {}

    bool ObjectTypeDescriptionGroup::IsIn(const ObjectTypeName& typeName) const
    {
        for (auto& loop : list)
            if (loop.typeName == typeName)
                return true;

        return false;
    }

    ObjectTypeDescriptionGroup::iterator ObjectTypeDescriptionGroup::begin()
    {
        return list.begin();
    }

    ObjectTypeDescriptionGroup::const_iterator ObjectTypeDescriptionGroup::begin() const
    {
        return list.begin();
    }

    ObjectTypeDescriptionGroup::iterator ObjectTypeDescriptionGroup::end()
    {
        return list.end();
    }

    ObjectTypeDescriptionGroup::const_iterator ObjectTypeDescriptionGroup::end() const
    {
        return list.end();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::ObjectTypeDescriptionGroup, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.list);
    }

    void Scribe<::Atmos::ObjectTypeDescriptionGroup, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}