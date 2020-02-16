#pragma once

#include "FileAsset.h"
#include "ImageAssetData.h"

namespace Atmos::Asset
{
    class ImageAsset final : public FileAsset<ImageAssetData, ImageAsset>
    {
    public:
        explicit ImageAsset(Init init);
        ImageAsset(ImageAsset&& arg) noexcept;

        ImageAsset& operator=(ImageAsset&& arg) noexcept;
    public:
        using Dimension = int;
        [[nodiscard]] Dimension Width() const;
        [[nodiscard]] Dimension Height() const;
    private:
        Dimension width = 0;
        Dimension height = 0;
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
            const ::Atmos::Name& fileName,
            ::Atmos::Asset::ImageAsset::DataPtr&& data);
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