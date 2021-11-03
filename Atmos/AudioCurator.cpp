#include "AudioCurator.h"

#include "SoundCore.h"
#include "Bounds.h"
#include "SoundFinished.h"

#include "FocusLost.h"
#include "FocusRegained.h"

#include "SpatialAlgorithms.h"

#include <Arca/All.h>

namespace Atmos::Audio
{
    Curator::Curator(Init init, Manager& manager) : Arca::Curator(init), manager(&manager)
    {
        init.owner.On<FocusLost>([this](const FocusLost&) { OnFocusLost(); });
        init.owner.On<FocusRegained>([this](const FocusRegained&) { OnFocusRegained(); });
    }

    void Curator::Handle(const Work& command)
    {
        const auto positionedBatch = Owner().Batch<Arca::All<Spatial::Bounds, SoundCore>>();
        for (auto sound = positionedBatch.begin(); sound != positionedBatch.end(); ++sound)
        {
            const auto id = sound.ID();
            const auto bounds = Owner().Find<Spatial::Bounds>(id);
            const auto position = ToPoint2D(bounds->Position());
            const auto mutableSound = MutablePointer().Of<SoundCore>(id);
            mutableSound->resource->SetPosition(position);
        }

        const auto doneResources = manager->DoneResources();
        const std::unordered_set<const Resource::Sound*> doneResourcesSet = { doneResources.begin(), doneResources.end() };
        manager->PruneDoneResources();

        const auto soundCoreBatch = Owner().Batch<SoundCore>();
        for (auto sound = soundCoreBatch.begin(); sound != soundCoreBatch.end(); ++sound)
        {
            const auto soundCore = Owner().Find<SoundCore>(sound.ID()).Get();
            const auto resource = soundCore ? soundCore->resource.get() : nullptr;
            if (doneResourcesSet.find(resource) != doneResourcesSet.end())
                Owner().Raise(SoundFinished{ sound.ID() });
        }
    }

    void Curator::Handle(const RestartSound& command)
    {
        auto sound = MutablePointer().Of<SoundCore>(command.id);
        if (!sound)
            return;

        sound->resource->Restart();
    }

    void Curator::Handle(const PauseSound& command)
    {
        auto sound = MutablePointer().Of<SoundCore>(command.id);
        if (!sound)
            return;

        sound->resource->Pause();
    }

    void Curator::Handle(const ResumeSound& command)
    {
        auto sound = MutablePointer().Of<SoundCore>(command.id);
        if (!sound)
            return;

        sound->resource->Resume();
    }

    void Curator::Handle(const ChangeSoundVolume& command)
    {
        auto sound = MutablePointer().Of<SoundCore>(command.id);
        if (!sound)
            return;

        const auto toVolume = command.to;
        sound->volume = toVolume;
        sound->resource->SetVolume(toVolume);
    }

    std::unique_ptr<Asset::Resource::Audio> Curator::Handle(const Asset::Resource::Create<Asset::Resource::Audio>& command)
    {
        return manager->CreateAssetResource(command.buffer, command.name);
    }

    std::unique_ptr<Resource::Sound> Curator::Handle(const Resource::CreateSound& command)
    {
        const auto asset = command.asset.Get();
        if (!asset)
            return {};

        return manager->CreateSoundResource(*asset->Resource(), command.volume);
    }

    void Curator::OnFocusLost()
    {
        for (auto& sound : Owner().Batch<SoundCore>())
            sound.resource->Pause();
    }

    void Curator::OnFocusRegained()
    {
        for (auto& sound : Owner().Batch<SoundCore>())
            sound.resource->Resume();
    }
}