#include "ShaderAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    ShaderAsset::ShaderAsset() = default;

    ShaderAsset::ShaderAsset(ShaderAsset&& arg) noexcept = default;

    ShaderAsset::ShaderAsset(const ::Inscription::BinaryTableData<ShaderAsset>& data) :
        FileAsset(data.base)
    {}

    ShaderAsset::DataT* ShaderAsset::Data()
    {
        return data.get();
    }

    const ShaderAsset::DataT* ShaderAsset::Data() const
    {
        return data.get();
    }

    void ShaderAsset::Initialize(const File::Name& fileName, DataPtr&& data)
    {
        SetFileName(fileName);
        this->data = std::move(data);
    }

    ShaderAssetData::~ShaderAssetData() = default;
}

namespace Arca
{
    const TypeName Traits<::Atmos::Asset::ShaderAsset>::typeName = "ShaderAsset";

    bool Traits<::Atmos::Asset::ShaderAsset>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::File::Name& fileName, ::Atmos::Asset::ShaderAsset::DataPtr&& data)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::ShaderAsset>(reliquary, fileName);
    }
}

namespace Inscription
{
    Scribe<::Atmos::Asset::ShaderAsset, BinaryArchive>::Table::Table()
    {
        MergeDataLinks({
            DataLink::Base(data.base) }
        );
    }
}