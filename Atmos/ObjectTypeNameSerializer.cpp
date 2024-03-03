
#include "ObjectTypeNameSerializer.h"

#include "Assert.h"

#include <Inscription/ContainerSize.h>

#include <Chroma/Function.h>

namespace Atmos
{
    void ObjectTypeNameSerializer::AddAll(const ObjectTypeDescriptionGroup& group)
    {
        auto currentId = ID(0);
        for (auto& loop : group)
        {
            if (Has(loop.typeName))
                continue;

            entries.push_back(Entry(loop.typeName, currentId));
            ++currentId;
        }
    }

    void ObjectTypeNameSerializer::Clear()
    {
        entries.clear();
    }

    void ObjectTypeNameSerializer::SaveAll(::Inscription::BinaryScribe& scribe)
    {
        ::Inscription::ContainerSize size(entries.size());
        scribe.Save(size);

        for (auto& loop : entries)
        {
            scribe.Save(loop.typeName);
            scribe.Save(loop.id);
        }
    }

    void ObjectTypeNameSerializer::LoadAll(::Inscription::BinaryScribe& scribe)
    {
        ::Inscription::ContainerSize size;
        scribe.Load(size);

        while (size-- > 0)
        {
            ObjectTypeName typeName;
            scribe.Load(typeName);

            ID id;
            scribe.Load(id);

            entries.push_back(Entry(typeName, id));
        }
    }

    void ObjectTypeNameSerializer::Save(const ObjectTypeName& typeName, ::Inscription::BinaryScribe& scribe)
    {
        auto found = std::find_if(entries.begin(), entries.end(), [&typeName](const Entry& entry) { return entry.typeName == typeName; });
        ATMOS_ASSERT(found != entries.end());
        
        scribe.Save(found->id);
    }

    void ObjectTypeNameSerializer::Load(ObjectTypeName& typeName, ::Inscription::BinaryScribe& scribe)
    {
        ID id;
        scribe.Load(id);

        auto found = std::find_if(entries.begin(), entries.end(), [&id](const Entry& entry) { return entry.id == id; });
        ATMOS_ASSERT(found != entries.end());

        typeName = found->typeName;
    }

    ObjectTypeNameSerializer::Entry::Entry(const ObjectTypeName& typeName, ID id) : typeName(typeName), id(id)
    {}

    bool ObjectTypeNameSerializer::Has(const ObjectTypeName& typeName)
    {
        for (auto& entryLoop : entries)
        {
            if (entryLoop.typeName == typeName)
                return true;
        }

        return false;
    }
}