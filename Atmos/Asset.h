#pragma once

#include "Object.h"

#include "Name.h"
#include "FileName.h"
#include "FilePath.h"

#include "ObjectScribe.h"

namespace Atmos
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

    template<>
    struct ObjectTraits<Asset> : ObjectTraitsBase<Asset>
    {
        static const ObjectTypeName typeName;
    };

    class FileAsset : public Asset
    {
    public:
        const FileName fileName;
    public:
        virtual ~FileAsset() = 0;

        ObjectTypeDescription TypeDescription() const override;
    protected:
        FileAsset(ObjectManager& manager, const FileName& fileName);
        FileAsset(const FileAsset& arg);
        FileAsset(const ::Inscription::BinaryTableData<FileAsset>& data);
    };

    template<>
    struct ObjectTraits<FileAsset> : ObjectTraitsBase<FileAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Asset> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Asset, BinaryArchive>
    {
        ::Atmos::Name name;
    };

    template<>
    class Scribe<::Atmos::Asset, BinaryArchive> :
        public ObjectScribe<::Atmos::Asset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };

    template<>
    struct TableData<::Atmos::FileAsset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::FileAsset, BinaryArchive>
    {
        ::Atmos::FileName fileName;
    };

    template<>
    class Scribe<::Atmos::FileAsset, BinaryArchive> :
        public ObjectScribe<::Atmos::FileAsset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };
}