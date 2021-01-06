#pragma once

#include "AssetCurator.h"

#include "ImageAsset.h"
#include "ImageAssetManager.h"

namespace Atmos::Asset
{
    class ImageCurator : public Curator<Image>
    {
    public:
        explicit ImageCurator(Init init, ImageManager& manager);
    public:
        using Curator<Image>::Handle;

        Resource::Loaded<Resource::Image> Handle(const Resource::LoadDataFromFile<Resource::Image>& command);
        Resource::Loaded<Resource::Image> Handle(const Resource::LoadDataFromMemory<Resource::Image>& command);
    private:
        ImageManager* manager;
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
        static TypeName TypeName() { return "Atmos::Asset::ImageCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Asset::FindByName<Atmos::Asset::Image>,
            Atmos::Asset::Resource::LoadDataFromFile<Atmos::Asset::Resource::Image>,
            Atmos::Asset::Resource::LoadDataFromMemory<Atmos::Asset::Resource::Image>>;
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