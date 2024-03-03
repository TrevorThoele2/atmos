#pragma once

#include "AssetWithResource.h"
#include "ImageAssetResource.h"
#include "ImageAssetSize.h"
#include "ImageAssetGridSize.h"
#include "AxisAlignedBox2D.h"

#include "LoadAssetsUserContext.h"
#include "CreateImageAssetResource.h"
#include "LoadImageAssetResource.h"

namespace Atmos::Asset
{
    class Image final : public AssetWithResource<Resource::Image, Image>
    {
    public:
        using Dimension = ImageSize::Dimension;
        using GridDimension = ImageGridSize::Dimension;
    public:
        Image(Init init,
            const Atmos::Name& name,
            ResourcePtr&& resource,
            ImageGridSize gridSize);
        Image(Init init, Arca::Serialization serialization);
        Image(Image&& arg) noexcept;

        Image& operator=(Image&& arg) noexcept;
    public:
        void Setup(ResourcePtr&& set, ImageSize size);
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
    struct Traits<Atmos::Asset::Image>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Asset::Image";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const Atmos::Asset::Image::ResourcePtr& data,
            Atmos::Asset::ImageGridSize gridSize);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Image> final
    {
    public:
        using ObjectT = Atmos::Asset::Image;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Image, Atmos::Asset::Image>>(
                object, archive);
            archive("gridSize", object.gridSize);
            if (archive.IsInput())
            {
                auto& assetUserContext = *archive.template UserContext<LoadAssetsUserContext>();

                auto extracted = assetUserContext.LoadImage(object.Name());
                if (extracted)
                {
                    using LoadResource = Atmos::Asset::Resource::LoadFromMemory<Atmos::Asset::Resource::Image>;
                    const auto loaded = object.Owner().Do(LoadResource
                    {
                        extracted->memory
                    });

                    using CreateResource = Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>;
                    auto resource = object.Owner().Do(CreateResource
                    {
                        loaded.buffer,
                        object.Name(),
                        loaded.size
                    });

                    object.Setup(std::move(resource), loaded.size);
                }
            }
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Image, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Image>;
    };
}