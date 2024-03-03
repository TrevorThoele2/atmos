#pragma once

#include "AssetCurator.h"

#include <FreeImage.h>
#include "ImageAsset.h"
#include "LoadImageAsset.h"

namespace Atmos::Asset
{
    class ImageAssetCurator : public AssetCurator<ImageAsset>
    {
    public:
        explicit ImageAssetCurator(Init init);
    public:
        using AssetCurator<ImageAsset>::Handle;

        LoadedImageAsset Handle(const LoadImageAsset& command);
    private:
        static std::optional<ImageAssetType> TypeFromFIF(FREE_IMAGE_FORMAT format);
    };

    template<>
    struct AssetCuratorTraits<ImageAsset> : AssetCuratorTraitsBase<ImageAsset>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::imageAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ImageAssetCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "ImageAssetCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::FindAsset<Atmos::Asset::ImageAsset>,
            Atmos::Asset::LoadImageAsset>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ImageAssetCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Asset::ImageAssetCurator, BinaryArchive>
    {};
}