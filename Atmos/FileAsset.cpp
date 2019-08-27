#include "FileAsset.h"

namespace Atmos::Asset
{
    File::Name FileAsset::FileName() const
    {
        return fileName;
    }

    FileAsset::FileAsset(FileAsset&& arg) noexcept : Asset(std::move(arg)), fileName(std::move(arg.fileName))
    {}

    FileAsset::FileAsset(const ::Inscription::BinaryTableData<FileAsset>& data) :
        Asset(data.base), fileName(data.fileName)
    {}

    void FileAsset::SetFileName(const File::Name& fileName)
    {
        SetName(fileName);
        this->fileName = fileName;
    }
}

namespace Inscription
{
    Scribe<::Atmos::Asset::FileAsset, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(data.base),
            DataLink::Auto(&ObjectT::fileName, &DataT::fileName) }
        );
    }
}