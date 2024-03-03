#include "Asset.h"

#include <Inscription/FileNameScribe.h>

namespace Atmos::Asset
{
    Name Asset::Name() const
    {
        return name;
    }

    Asset::Asset(Asset&& arg) noexcept : name(std::move(arg.name))
    {}

    Asset::Asset(const ::Inscription::BinaryTableData<Asset>& data) : name(data.name)
    {}

    void Asset::SetName(const Atmos::Name& name)
    {
        this->name = name;
    }
}

namespace Inscription
{
    Scribe<::Atmos::Asset::Asset, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Auto(&ObjectT::name, &DataT::name) }
        );
    }
}