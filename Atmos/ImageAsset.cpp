#include "ImageAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    auto ImageAsset::Width() const -> Dimension
    {
        return width;
    }

    auto ImageAsset::Height() const -> Dimension
    {
        return height;
    }

    ImageAsset::ImageAsset() = default;

    ImageAsset::ImageAsset(const ::Inscription::BinaryTableData<ImageAsset>& data) :
        FileAsset(data.base), width(data.width), height(data.height)
    {}

    ImageAsset::DataT* ImageAsset::Data()
    {
        return data.get();
    }

    const ImageAsset::DataT* ImageAsset::Data() const
    {
        return data.get();
    }

    void ImageAsset::Initialize(const File::Name& fileName, DataPtr&& data)
    {
        SetFileName(fileName);
        this->data = std::move(data);
    }

    ImageAssetData::~ImageAssetData() = default;
}

namespace Arca
{
    bool Traits<::Atmos::Asset::ImageAsset>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::File::Name& fileName, ::Atmos::Asset::ImageAsset::DataPtr&& data)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::ImageAsset>(reliquary, fileName);
    }
}

namespace Inscription
{
    Scribe<::Atmos::Asset::ImageAsset, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(data.base),
            DataLink::Auto(&ObjectT::width, &DataT::width),
            DataLink::Auto(&ObjectT::height, &DataT::height) }
        );
    }
}