#include "MaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    auto MaterialAsset::Columns() const -> GridDimension
    {
        return columns;
    }

    auto MaterialAsset::Rows() const -> GridDimension
    {
        return rows;
    }

    ImageAsset* MaterialAsset::Image() const
    {
        return image;
    }

    ShaderAsset* MaterialAsset::Shader() const
    {
        return shader;
    }

    MaterialAsset::MaterialAsset() = default;

    MaterialAsset::MaterialAsset(const ::Inscription::BinaryTableData<MaterialAsset>& data) :
        Asset(data.base)
    {}

    void MaterialAsset::Initialize(const Atmos::Name& name)
    {
        SetName(name);
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::MaterialAsset>::ShouldCreate(Reliquary& reliquary, const ::Atmos::Name& name)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::MaterialAsset>(reliquary, name);
    }
}

namespace Inscription
{
    Scribe<::Atmos::Asset::MaterialAsset, BinaryArchive>::Table::Table()
    {
        MergeDataLinks({
            DataLink::Base(data.base) }
        );
    }
}