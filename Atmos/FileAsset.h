#pragma once

#include "Asset.h"

namespace Atmos::Asset
{
    class FileAsset : public Asset
    {
    public:
        [[nodiscard]] File::Name FileName() const;
    protected:
        FileAsset() = default;
        FileAsset(FileAsset&& arg) noexcept;
        explicit FileAsset(const ::Inscription::BinaryTableData<FileAsset>& data);

        void SetFileName(const File::Name& fileName);
    private:
        File::Name fileName;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::FileAsset, BinaryArchive> :
        TableDataBase<::Atmos::Asset::FileAsset, BinaryArchive>
    {
        Base<::Atmos::Asset::Asset> base;
        ::Atmos::File::Name fileName;
    };

    template<>
    class Scribe<::Atmos::Asset::FileAsset, BinaryArchive> final :
        public TableScribe<::Atmos::Asset::FileAsset, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    };
}