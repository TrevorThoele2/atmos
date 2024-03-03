#include "ImageAsset.h"

#include "ShouldCreateAsset.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Asset
{
    Image::Image(
        Arca::RelicInit init,
        const Atmos::Name& name,
        ResourcePtr&& resource,
        ImageGridSize gridSize)
        :
        AssetWithResource(init, name, std::move(resource)),
        init(init),
        gridSize(gridSize)
    {
        const auto useResource = Resource();
        size = useResource ? useResource->Size() : Spatial::Size2D{};
    }

    Image::Image(Arca::RelicInit init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization),
        init(init)
    {}

    Image::Image(Image&& arg) noexcept :
        AssetWithResource(std::move(arg)),
        init(arg.init),
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

    void Image::Setup(ResourcePtr&& set, Spatial::Size2D size)
    {
        SetResource(std::move(set));
        this->size = size;
    }

    auto Image::Width() const -> Dimension
    {
        return size.width;
    }

    auto Image::Height() const -> Dimension
    {
        return size.height;
    }

    Spatial::Size2D Image::Size() const
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

        return Spatial::ToAxisAlignedBox2D
        (
            column * sliceSize.width,
            row * sliceSize.height,
            column * sliceSize.width + sliceSize.width,
            row * sliceSize.height + sliceSize.height
        );
    }

    Spatial::Size2D Image::SliceSize() const
    {
        const auto gridSize = UsableGridSize();

        const auto indexWidth = static_cast<float>(Width()) / gridSize.columns;
        const auto indexHeight = static_cast<float>(Height()) / gridSize.rows;

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
        const Atmos::Name& name,
        const Atmos::Asset::Image::ResourcePtr&,
        Atmos::Asset::ImageGridSize)
    {
        return Atmos::Asset::ShouldCreate<Atmos::Asset::Image>(reliquary, name);
    }
}