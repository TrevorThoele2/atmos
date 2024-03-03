#pragma once

#include <vector>

#include "ObjectTypeName.h"
#include "ObjectTypeDescriptionGroup.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    class ObjectTypeNameSerializer
    {
    public:
        void AddAll(const ObjectTypeDescriptionGroup& group);

        void Clear();
    public:
        void SaveAll(::Inscription::BinaryScribe& scribe);
        void LoadAll(::Inscription::BinaryScribe& scribe);
        void Save(const ObjectTypeName& typeName, ::Inscription::BinaryScribe& scribe);
        void Load(ObjectTypeName& typeName, ::Inscription::BinaryScribe& scribe);
    private:
        typedef size_t ID;

        struct Entry
        {
            ObjectTypeName typeName;
            ID id;
            Entry(const ObjectTypeName& typeName, ID id);
        };
    private:
        std::vector<Entry> entries;

        bool Has(const ObjectTypeName& typeName);
    };

    static ObjectTypeNameSerializer objectTypeNameSerializer;
}