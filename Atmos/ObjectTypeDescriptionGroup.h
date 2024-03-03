#pragma once

#include <vector>

#include "ObjectTypeDescription.h"

#include "Serialization.h"

namespace Atmos
{
    class ObjectTypeDescriptionGroup
    {
    public:
        typedef std::vector<ObjectTypeDescription> List;

        typedef List::iterator iterator;
        typedef List::const_iterator const_iterator;
    public:
        ObjectTypeDescriptionGroup();
        ObjectTypeDescriptionGroup(const List& list);
        ObjectTypeDescriptionGroup(List&& list);

        bool IsIn(const ObjectTypeName& typeName) const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
    private:
        List list;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}