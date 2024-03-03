#pragma once

#include "AssetCurator.h"
#include "AudioAsset.h"
#include "AssetResourceManager.h"

namespace Atmos::Asset
{
    class AudioCurator : public Curator<Audio>
    {
    public:
        explicit AudioCurator(Init init, Resource::Manager& resourceManager);
    public:
        using Curator<Audio>::Handle;

        Resource::LoadedData<Resource::Audio> Handle(const Resource::LoadData<Resource::Audio>& command);
    private:
        Resource::Manager* resourceManager;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::AudioCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Asset::AudioCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::FindByName<Atmos::Asset::Audio>,
            Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Audio>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Asset::AudioCurator, Archive> final
    {
        using Category = AssetScribeCategory<Atmos::Asset::AudioCurator>;
    };
}