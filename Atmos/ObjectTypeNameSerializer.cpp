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

    void ObjectTypeNameSerializer::SaveAll(::Inscription::OutputBinaryArchive& archive)
    {
        ::Inscription::ContainerSize size(entries.size());
        archive(size);

        for (auto& loop : entries)
        {
            archive(loop.typeName);
            archive(loop.id);
        }
    }

    void ObjectTypeNameSerializer::LoadAll(::Inscription::InputBinaryArchive& archive)
    {
        ::Inscription::ContainerSize size;
        archive(size);

        while (size-- > 0)
        {
            ObjectTypeName typeName;
            archive(typeName);

            ID id;
            archive(id);

            entries.push_back(Entry(typeName, id));
        }
    }

    void ObjectTypeNameSerializer::Save(const ObjectTypeName& typeName, ::Inscription::OutputBinaryArchive& archive)
    {
        auto found = std::find_if(
            entries.begin(),
            entries.end(),
            [&typeName](const Entry& entry) { return entry.typeName == typeName; });
        ATMOS_ASSERT(found != entries.end());
        
        archive(found->id);
    }

    void ObjectTypeNameSerializer::Load(ObjectTypeName& typeName, ::Inscription::InputBinaryArchive& archive)
    {
        ID id;
        archive(id);

        auto found = std::find_if(
            entries.begin(),
            entries.end(),
            [&id](const Entry& entry) { return entry.id == id; });
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