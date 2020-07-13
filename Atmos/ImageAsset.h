#pragma once

#include "AssetWithResource.h"
#include "ImageAssetResource.h"
#include "ImageAssetSize.h"
#include "ImageAssetGridSize.h"
#include "AxisAlignedBox2D.h"

namespace Atmos::Asset
{
    class Image final : public AssetWithResource<Resource::Image, Image>
    {
    public:
        using Dimension = ImageSize::Dimension;
        using GridDimension = ImageGridSize::Dimension;
    public:
        Image(Init init,
            const ::Atmos::Name& name,
            ResourcePtr&& resource,
            ImageSize size,
            ImageGridSize gridSize);
        Image(Init init, Arca::Serialization serialization);
        Image(Image&& arg) noexcept;

        Image& operator=(Image&& arg) noexcept;
    public:
        [[nodiscard]] Dimension Width() const;
        [[nodiscard]] Dimension Height() const;
        [[nodiscard]] ImageSize Size() const;

        [[nodiscard]] GridDimension Columns() const;
        [[nodiscard]] GridDimension Rows() const;
        [[nodiscard]] ImageGridSize GridSize() const;

        [[nodiscard]] Spatial::AxisAlignedBox2D Slice(int index) const;
        [[nodiscard]] Spatial::Size2D SliceSize() const;
    private:
        [[nodiscard]] ImageGridSize UsableGridSize() const;
    private:
        ImageSize size;
        ImageGridSize gridSize;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::Image>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Asset::Image";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::Name& name,
            const ::Atmos::Asset::Image::ResourcePtr& data,
            ::Atmos::Asset::ImageSize size,
            ::Atmos::Asset::ImageGridSize gridSize);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::Image, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::Image, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}