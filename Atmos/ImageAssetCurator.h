#pragma once

#include "AssetCurator.h"

#include "FreeImageIncludes.h"
#include "ImageAsset.h"
#include "LoadImageAsset.h"
#include "GraphicsManager.h"

namespace Atmos::Asset
{
    class ImageCurator : public Curator<Image>
    {
    public:
        explicit ImageCurator(Init init);
    public:
        using Curator<Image>::Handle;

        Loaded<Image> Handle(const Load<Image>& command);
    private:
        static std::optional<ImageType> TypeFromFIF(FREE_IMAGE_FORMAT format);
    };

    template<>
    struct CuratorTraits<Image> : CuratorTraitsBase<Image>
    {
        constexpr static DebugStatisticsSize debugStatisticsSize = &Debug::Statistics::Memory::imageAssetSize;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ImageCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "ImageAssetCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::Find<Atmos::Asset::Image>,
            Atmos::Asset::Load<Atmos::Asset::Image>>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ImageCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Asset::ImageCurator, BinaryArchive>
    {};
}