#pragma once

#include "Name.h"
#include "FileName.h"
#include "FilePath.h"

#include "Serialization.h"

namespace Atmos::Asset
{
    class Asset
    {
    public:
        [[nodiscard]] Name Name() const;
    protected:
        Asset() = default;
        Asset(Asset&& arg) noexcept;
        explicit Asset(const ::Inscription::BinaryTableData<Asset>& data);

        void SetName(const Atmos::Name& name);
    private:
        Atmos::Name name;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Asset::Asset, BinaryArchive> :
        TableDataBase<::Atmos::Asset::Asset, BinaryArchive>
    {
        ::Atmos::Name name;
    };

    template<>
    class Scribe<::Atmos::Asset::Asset, BinaryArchive> final :
        public TableScribe<::Atmos::Asset::Asset, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    };
}