#include "FileAsset.h"

namespace Atmos::Asset
{
    FileAsset::~FileAsset()
    {}

    ObjectTypeDescription FileAsset::TypeDescription() const
    {
        return ObjectTraits<FileAsset>::TypeDescription();
    }

    FileAsset::FileAsset(ObjectManager& manager, const File::Name& fileName) :
        Asset(manager, fileName), fileName(fileName)
    {}

    FileAsset::FileAsset(const FileAsset& arg) :
        Asset(arg), fileName(arg.fileName)
    {}

    FileAsset::FileAsset(const ::Inscription::BinaryTableData<FileAsset>& data) :
        Asset(std::get<0>(data.bases)), fileName(data.fileName)
    {}
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Asset::FileAsset>::typeName = "FileAsset";
}

namespace Inscription
{
    Scribe<::Atmos::Asset::FileAsset, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::fileName, &DataT::fileName) });
    }
}