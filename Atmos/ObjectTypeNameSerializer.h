#pragma once

#include <vector>

#include "ObjectTypeName.h"
#include "ObjectTypeDescriptionGroup.h"

#include <Inscription/OutputBinaryArchive.h>
#include <Inscription/InputBinaryArchive.h>

namespace Atmos
{
    class ObjectTypeNameSerializer
    {
    public:
        void AddAll(const ObjectTypeDescriptionGroup& group);

        void Clear();
    public:
        void SaveAll(::Inscription::OutputBinaryArchive& archive);
        void LoadAll(::Inscription::InputBinaryArchive& archive);
        void Save(const ObjectTypeName& typeName, ::Inscription::OutputBinaryArchive& archive);
        void Load(ObjectTypeName& typeName, ::Inscription::InputBinaryArchive& archive);
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