#pragma once

#include <Arca/Curator.h>

#include "AudioManager.h"

#include "CreateAudioAssetResource.h"

namespace Atmos::Audio
{
    class AudioCurator final : public Arca::Curator
    {
    public:
        explicit AudioCurator(Init init, AudioManager& manager);
    public:
        std::unique_ptr<Asset::Resource::Audio> Handle(const Asset::Resource::Create<Asset::Resource::Audio>& command);
    private:
        AudioManager* manager;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::AudioCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Audio::AudioCurator";
        using HandledCommands = HandledCommands<
            Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Audio::AudioCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Audio::AudioCurator>;
    };
}