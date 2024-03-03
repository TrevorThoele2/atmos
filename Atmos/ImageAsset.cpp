#include "ImageAsset.h"

#include "LoadImageAsset.h"
#include "CreateImageAssetResource.h"
#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Image::Image(
        Init init,
        const ::Atmos::Name& name,
        ResourcePtr&& resource,
        ImageSize size,
        ImageGridSize gridSize)
        :
        AssetWithResource(init, name, std::move(resource)),
        size(size),
        gridSize(gridSize)
    {}

    Image::Image(Init init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization)
    {}

    Image::Image(Image&& arg) noexcept :
        AssetWithResource(std::move(arg)),
        size(arg.size),
        gridSize(arg.gridSize)
    {}

    Image& Image::operator=(Image&& arg) noexcept
    {
        AssetWithResource::operator=(std::move(arg));
        size = arg.size;
        gridSize = arg.gridSize;
        return *this;
    }

    auto Image::Width() const -> Dimension
    {
        return size.width;
    }

    auto Image::Height() const -> Dimension
    {
        return size.height;
    }

    ImageSize Image::Size() const
    {
        return size;
    }

    auto Image::Columns() const -> GridDimension
    {
        return gridSize.columns;
    }

    auto Image::Rows() const -> GridDimension
    {
        return gridSize.rows;
    }

    ImageGridSize Image::GridSize() const
    {
        return gridSize;
    }

    Spatial::AxisAlignedBox2D Image::Slice(int index) const
    {
        const auto gridSize = UsableGridSize();
        const auto sliceSize = SliceSize();

        const auto column = index % gridSize.columns;
        const auto row = index / gridSize.rows;

        return Spatial::AxisAlignedBox2D
        {
            column * sliceSize.width,
            row * sliceSize.height,
            column * sliceSize.width + sliceSize.width,
            row * sliceSize.height + sliceSize.height
        };
    }

    Spatial::Size2D Image::SliceSize() const
    {
        const auto gridSize = UsableGridSize();

        const auto indexWidth = static_cast<float>(Width() / gridSize.columns);
        const auto indexHeight = static_cast<float>(Height() / gridSize.rows);

        return Spatial::Size2D{ indexWidth, indexHeight };
    }

    ImageGridSize Image::UsableGridSize() const
    {
        const auto columns = Columns() > 0 ? Columns() : 1;
        const auto rows = Rows() ? Rows() : 1;
        return ImageGridSize{ columns, rows };
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::Image>::ShouldCreate(
        Reliquary& reliquary,
        const ::Atmos::Name& name,
        const ::Atmos::Asset::Image::ResourcePtr&,
        ::Atmos::Asset::ImageSize,
        ::Atmos::Asset::ImageGridSize)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::Image>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::Image, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Image, Atmos::Asset::Image>>(
            object, archive);
        archive(object.size);
        archive(object.gridSize);
        if (archive.IsInput())
        {
            const auto filePath = std::filesystem::current_path() / "images" / object.Name();
            const auto loaded = object.Owner().Do(Atmos::Asset::Load<Atmos::Asset::Image>{ filePath });
            object.resource = object.Owner().Do(Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>{
                loaded.buffer,
                object.Name(),
                loaded.size });
        }
    }
}