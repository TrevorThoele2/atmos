#include "ImageAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    ImageAsset& ImageAsset::operator=(ImageAsset&& arg) noexcept
    {
        width = arg.width;
        height = arg.height;
        FileAsset::operator=(std::move(arg));
        return *this;
    }

    auto ImageAsset::Width() const -> Dimension
    {
        return width;
    }

    auto ImageAsset::Height() const -> Dimension
    {
        return height;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::ImageAsset>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::Name& fileName, ::Atmos::Asset::ImageAsset::DataPtr&& data)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::ImageAsset>(reliquary, fileName);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::ImageAsset, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::FileAsset<Atmos::Asset::ImageAssetData, Atmos::Asset::ImageAsset>>(
            object, archive);
        archive(object.width);
        archive(object.height);
    }
}