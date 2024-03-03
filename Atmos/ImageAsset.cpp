#include "ImageAsset.h"

#include "LoadImageAsset.h"
#include "CreateImageAssetData.h"
#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Image::Image(Init init) : FileAsset(init)
    {}

    Image::Image(
        Init init,
        const ::Atmos::Name& name,
        DataPtr&& data,
        ImageSize size,
        ImageGridSize gridSize)
        :
        FileAsset(init, name, std::move(data)),
        size(size),
        gridSize(gridSize)
    {}

    Image::Image(Image&& arg) noexcept :
        FileAsset(std::move(arg)),
        size(arg.size),
        gridSize(arg.gridSize)
    {}

    Image& Image::operator=(Image&& arg) noexcept
    {
        FileAsset::operator=(std::move(arg));
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

    AxisAlignedBox2D Image::Slice(int index) const
    {
        const auto gridSize = UsableGridSize();
        const auto sliceSize = SliceSize();

        const auto column = index % gridSize.columns;
        const auto row = index / gridSize.rows;

        return AxisAlignedBox2D
        {
            column * sliceSize.width,
            row * sliceSize.height,
            column * sliceSize.width + sliceSize.width,
            row * sliceSize.height + sliceSize.height
        };
    }

    Size2D Image::SliceSize() const
    {
        const auto gridSize = UsableGridSize();

        const auto indexWidth = static_cast<float>(Width() / gridSize.columns);
        const auto indexHeight = static_cast<float>(Height() / gridSize.rows);

        return Size2D{ indexWidth, indexHeight };
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
        const ::Atmos::Asset::Image::DataPtr&,
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
        BaseScriven<Atmos::Asset::FileAsset<Atmos::Asset::ImageData, Atmos::Asset::Image>>(
            object, archive);
        archive(object.size);
        archive(object.gridSize);
        if (archive.IsInput())
        {
            const auto filePath = std::filesystem::current_path() / "Images" / object.Name();
            const auto loaded = object.Owner().Do(Atmos::Asset::LoadImage{ filePath });
            object.data = object.Owner().Do(Atmos::Asset::CreateData<Atmos::Asset::ImageData>{
                loaded.buffer,
                object.Name(),
                loaded.size });
        }
    }
}