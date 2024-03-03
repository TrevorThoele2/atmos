#pragma once

#include "AssetCurator.h"

#include "ImageAsset.h"
#include "AssetResourceManager.h"

namespace Atmos::Asset
{
    class ImageCurator : public Curator<Image>
    {
    public:
        explicit ImageCurator(Init init, Resource::Manager& resourceManager);
    public:
        using Curator<Image>::Handle;

        Resource::LoadedData<Resource::Image> Handle(const Resource::LoadData<Resource::Image>& command);
    private:
        Resource::Manager* resourceManager;
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
            Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Image>>;
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