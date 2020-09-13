#pragma once

#include "AssetCurator.h"

#include "FreeImageIncludes.h"
#include "ImageAsset.h"
#include "LoadImageAssetResource.h"

namespace Atmos::Asset
{
    class ImageCurator : public Curator<Image>
    {
    public:
        explicit ImageCurator(Init init);
    public:
        using Curator<Image>::Handle;

        Resource::Loaded<Resource::Image> Handle(const Resource::LoadFromFile<Resource::Image>& command);
        Resource::Loaded<Resource::Image> Handle(const Resource::LoadFromMemory<Resource::Image>& command);
    private:
        static Resource::Loaded<Resource::Image> ProcessBitmap(FIBITMAP* loadedBitmap, FREE_IMAGE_FORMAT format);

        static FREE_IMAGE_FORMAT FIFFor(const String& filePath);
        static FREE_IMAGE_FORMAT FIFFor(FIMEMORY& memory, int size);
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
        static inline const TypeName typeName = "Atmos::Asset::ImageCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::Find<Atmos::Asset::Image>,
            Atmos::Asset::Resource::LoadFromFile<Atmos::Asset::Resource::Image>,
            Atmos::Asset::Resource::LoadFromMemory<Atmos::Asset::Resource::Image>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ImageCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ImageCurator>;
    };
}