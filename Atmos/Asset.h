#pragma once

#include "Object.h"

#include "Name.h"
#include "FileName.h"
#include "FilePath.h"

#include "ObjectScribe.h"

namespace Atmos::Asset
{
    class Asset : public Object
    {
    public:
        const Name name;
    public:
        Asset(ObjectManager& manager, const Name& name);
        Asset(const Asset& arg);
        Asset(const ::Inscription::BinaryTableData<Asset>& data);

        ObjectTypeDescription TypeDescription() const override;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Asset::Asset> : ObjectTraitsBase<Asset::Asset>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::Asset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Asset::Asset, BinaryArchive>
    {
        ::Atmos::Name name;
    };

    template<>
    class Scribe<::Atmos::Asset::Asset, BinaryArchive> :
        public ObjectScribe<::Atmos::Asset::Asset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };
}