#pragma once

#include "FileAsset.h"
#include "ImageAssetData.h"

namespace Atmos::Asset
{
    class ImageAsset final : public FileAsset<ImageAssetData, ImageAsset>
    {
    public:
        using Dimension = int;
        using GridDimension = int;
    public:
        explicit ImageAsset(Init init);
        ImageAsset(Init init,
            const ::Atmos::Name& name,
            DataPtr&& data,
            GridDimension columns,
            GridDimension rows);
        ImageAsset(ImageAsset&& arg) noexcept;

        ImageAsset& operator=(ImageAsset&& arg) noexcept;
    public:
        [[nodiscard]] Dimension Width() const;
        [[nodiscard]] Dimension Height() const;

        [[nodiscard]] GridDimension Columns() const;
        [[nodiscard]] GridDimension Rows() const;
    private:
        GridDimension columns = 0;
        GridDimension rows = 0;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Asset::ImageAsset>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "ImageAsset";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const ::Atmos::Name& name,
            ::Atmos::Asset::ImageAsset::DataPtr&& data,
            ::Atmos::Asset::ImageAsset::GridDimension columns,
            ::Atmos::Asset::ImageAsset::GridDimension rows);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::ImageAsset, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::ImageAsset, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}