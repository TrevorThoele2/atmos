#pragma once

#include <Arca/Curator.h>

#include "AudioManager.h"

#include "RestartSound.h"
#include "PauseSound.h"
#include "ResumeSound.h"
#include "ChangeSoundVolume.h"
#include "CreateAudioAssetResource.h"
#include "CreateSoundResource.h"
#include "SoundResource.h"
#include "SoundCore.h"

#include "Work.h"

namespace Atmos::Audio
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init, Manager& manager);
    public:
        void Handle(const Work& command);

        void Handle(const RestartSound& command);
        void Handle(const PauseSound& command);
        void Handle(const ResumeSound& command);
        void Handle(const ChangeSoundVolume& command);

        std::unique_ptr<Asset::Resource::Audio> Handle(const Asset::Resource::Create<Asset::Resource::Audio>& command);
        std::unique_ptr<Resource::Sound> Handle(const Resource::CreateSound& command);
    private:
        Manager* manager;

        void OnFocusLost();
        void OnFocusRegained();
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::Curator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Audio::Curator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Audio::RestartSound,
            Atmos::Audio::PauseSound,
            Atmos::Audio::ResumeSound,
            Atmos::Audio::ChangeSoundVolume,
            Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Audio>,
            Atmos::Audio::Resource::CreateSound>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Audio::Curator> final
    {
    public:
        using ObjectT = Atmos::Audio::Curator;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive&)
        {
            const auto batch = object.Owner().Batch<Atmos::Audio::SoundCore>();
            for(auto soundCore = batch.begin(); soundCore != batch.end(); ++soundCore)
            {
                auto mutableSoundCore = object.MutablePointer().Of<Atmos::Audio::SoundCore>(soundCore.ID());
                mutableSoundCore->resource =
                    object.manager->CreateSoundResource(*mutableSoundCore->asset->Resource(), mutableSoundCore->volume);
            }
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Audio::Curator, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Audio::Curator>;
    };
}