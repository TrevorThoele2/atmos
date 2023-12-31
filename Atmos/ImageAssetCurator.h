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
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Asset::ImageCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::FindByName<Atmos::Asset::Image>,
            Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Image>>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Asset::ImageCurator, Format> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::ImageCurator>;
    };
}