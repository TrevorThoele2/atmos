#pragma once

#include "Asset.h"

namespace Atmos::Asset
{
    class FileAsset : public Asset
    {
    public:
        const File::Name fileName;
    public:
        virtual ~FileAsset() = 0;

        ObjectTypeDescription TypeDescription() const override;
    protected:
        FileAsset(ObjectManager& manager, const File::Name& fileName);
        FileAsset(const FileAsset& arg);
        FileAsset(const ::Inscription::BinaryTableData<FileAsset>& data);
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Asset::FileAsset> : ObjectTraitsBase<Asset::FileAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Asset::Asset> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::FileAsset, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Asset::FileAsset, BinaryArchive>
    {
        ::Atmos::File::Name fileName;
    };

    template<>
    class Scribe<::Atmos::Asset::FileAsset, BinaryArchive> :
        public ObjectScribe<::Atmos::Asset::FileAsset, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };
}