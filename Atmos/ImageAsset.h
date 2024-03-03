#pragma once

#include "AssetWithResource.h"
#include "ImageAssetResource.h"
#include "Size2D.h"
#include "ImageAssetGridSize.h"
#include "AxisAlignedBox2D.h"

#include "LoadAssetsUserContext.h"
#include "CreateImageAssetResource.h"
#include "LoadImageAssetResourceData.h"

namespace Atmos::Asset
{
    class Image final : public AssetWithResource<Resource::Image>
    {
    public:
        using Dimension = Spatial::Size2D::Value;
        using GridDimension = ImageGridSize::Dimension;
    public:
        Image(
            Arca::RelicInit init,
            const Atmos::Name& name,
            ResourcePtr&& resource,
            ImageGridSize gridSize);
        Image(Arca::RelicInit init, Arca::Serialization serialization);
        Image(Image&& arg) noexcept;

        Image& operator=(Image&& arg) noexcept;

        bool operator==(const Image& arg) const;
        bool operator!=(const Image& arg) const;
    public:
        void Setup(ResourcePtr&& set, Spatial::Size2D size);
    public:
        [[nodiscard]] Dimension Width() const;
        [[nodiscard]] Dimension Height() const;
        [[nodiscard]] Spatial::Size2D Size() const;

        [[nodiscard]] GridDimension Columns() const;
        [[nodiscard]] GridDimension Rows() const;
        [[nodiscard]] ImageGridSize GridSize() const;

        [[nodiscard]] Spatial::AxisAlignedBox2D Slice(int index) const;
        [[nodiscard]] Spatial::Size2D SliceSize() const;
    private:
        [[nodiscard]] ImageGridSize UsableGridSize() const;
    private:
        Arca::RelicInit init;
    private:
        Spatial::Size2D size;
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
        static TypeName TypeName() { return "Atmos::Asset::Image"; }
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
            BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Image>>(
                object, archive);
            archive("gridSize", object.gridSize);
            if (archive.IsInput())
            {
                auto& assetUserContext = *archive.template UserContext<LoadAssetsUserContext>();

                auto extracted = assetUserContext.LoadImageData(object.Name());
                if (extracted)
                {
                    using LoadResource = Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Image>;
                    const auto loaded = object.init.owner.Do(LoadResource
                    {
                        extracted->memory
                    });

                    using CreateResource = Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Image>;
                    auto resource = object.init.owner.Do(CreateResource
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