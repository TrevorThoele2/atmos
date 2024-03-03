#include "Asset.h"

#include <Inscription/FileNameScribe.h>

namespace Atmos
{
    const ObjectTypeName ObjectTraits<FileAsset>::typeName = "FileAsset";

    Asset::Asset(ObjectManager& manager, const Name& name) : Object(manager), name(name)
    {}

    Asset::Asset(const Asset& arg) : Object(arg), name(arg.name)
    {}

    Asset::Asset(const ::Inscription::BinaryTableData<Asset>& data) :
        Object(std::get<0>(data.bases)), name(data.name)
    {}

    ObjectTypeDescription Asset::TypeDescription() const
    {
        return ObjectTraits<Asset>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<Asset>::typeName = "Asset";

    FileAsset::~FileAsset()
    {}

    ObjectTypeDescription FileAsset::TypeDescription() const
    {
        return ObjectTraits<FileAsset>::TypeDescription();
    }

    FileAsset::FileAsset(ObjectManager& manager, const FileName& fileName) :
        Asset(manager, fileName), fileName(fileName)
    {}

    FileAsset::FileAsset(const FileAsset& arg) :
        Asset(arg), fileName(arg.fileName)
    {}

    FileAsset::FileAsset(const ::Inscription::BinaryTableData<FileAsset>& data) :
        Asset(std::get<0>(data.bases)), fileName(data.fileName)
    {}
}

namespace Inscription
{
    Scribe<::Atmos::Asset, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::name, &DataT::name) });
    }

    Scribe<::Atmos::FileAsset, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::fileName, &DataT::fileName) });
    }
}