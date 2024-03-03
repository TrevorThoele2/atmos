#include "ImageAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    ImageAsset::ImageAsset(Init init) : FileAsset(init)
    {}

    ImageAsset::ImageAsset(
        Init init,
        const ::Atmos::Name& name,
        DataPtr&& data,
        GridDimension columns,
        GridDimension rows)
        :
        FileAsset(init, name, std::move(data)),
        columns(columns),
        rows(rows)
    {}

    ImageAsset::ImageAsset(ImageAsset&& arg) noexcept :
        FileAsset(std::move(arg)),
        columns(arg.columns),
        rows(arg.rows)
    {}

    ImageAsset& ImageAsset::operator=(ImageAsset&& arg) noexcept
    {
        FileAsset::operator=(std::move(arg));
        columns = arg.columns;
        rows = arg.rows;
        return *this;
    }

    auto ImageAsset::Width() const -> Dimension
    {
        if (!HasFileData())
            return 0;

        return FileData()->Width();
    }

    auto ImageAsset::Height() const -> Dimension
    {
        if (!HasFileData())
            return 0;

        return FileData()->Height();
    }

    auto ImageAsset::Columns() const -> GridDimension
    {
        return columns;
    }

    auto ImageAsset::Rows() const -> GridDimension
    {
        return rows;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::ImageAsset>::ShouldCreate(
        Reliquary& reliquary,
        const ::Atmos::Name& name,
        ::Atmos::Asset::ImageAsset::DataPtr&&,
        ::Atmos::Asset::ImageAsset::GridDimension,
        ::Atmos::Asset::ImageAsset::GridDimension)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::ImageAsset>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::ImageAsset, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::FileAsset<Atmos::Asset::ImageAssetData, Atmos::Asset::ImageAsset>>(
            object, archive);
    }
}