#pragma once

#include <vector>

#include "ObjectTypeName.h"
#include "ObjectTypeDescriptionGroup.h"

#include <Inscription/OutputBinaryScribe.h>
#include <Inscription/InputBinaryScribe.h>

namespace Atmos
{
    class ObjectTypeNameSerializer
    {
    public:
        void AddAll(const ObjectTypeDescriptionGroup& group);

        void Clear();
    public:
        void SaveAll(::Inscription::OutputBinaryScribe& scribe);
        void LoadAll(::Inscription::InputBinaryScribe& scribe);
        void Save(const ObjectTypeName& typeName, ::Inscription::OutputBinaryScribe& scribe);
        void Load(ObjectTypeName& typeName, ::Inscription::InputBinaryScribe& scribe);
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