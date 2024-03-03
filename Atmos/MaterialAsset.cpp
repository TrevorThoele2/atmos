#include "MaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    MaterialAsset::MaterialAsset(Init init) :
        Asset(init)
    {}

    MaterialAsset::MaterialAsset(Init init, const Atmos::Name& name, GridDimension columns, GridDimension rows) :
        Asset(init, name), columns(columns), rows(rows)
    {}

    MaterialAsset::MaterialAsset(MaterialAsset&& arg) noexcept :
        Asset(std::move(arg)), columns(arg.columns), rows(arg.rows)
    {}

    MaterialAsset& MaterialAsset::operator=(MaterialAsset && arg) noexcept
    {
        columns = arg.columns;
        rows = arg.rows;
        image = std::move(arg.image);
        shader = std::move(arg.shader);
        Asset::operator=(std::move(arg));
        return *this;
    }

    auto MaterialAsset::Columns() const -> GridDimension
    {
        return columns;
    }

    auto MaterialAsset::Rows() const -> GridDimension
    {
        return rows;
    }

    Arca::RelicIndex<ImageAsset> MaterialAsset::Image() const
    {
        return image;
    }

    Arca::RelicIndex<ShaderAsset> MaterialAsset::Shader() const
    {
        return shader;
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
    void Scribe<Atmos::Asset::MaterialAsset, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::Asset<Atmos::Asset::MaterialAsset>>(object, archive);
        archive(object.columns);
        archive(object.rows);
    }
}