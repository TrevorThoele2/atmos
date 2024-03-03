
#include "MasterSoundSystem.h"

#include "ObjectManager.h"
#include "AudioSystem.h"
#include "InitializationFileSystem.h"

namespace Atmos
{
    MasterSoundSystem::MasterSoundSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    void MasterSoundSystem::SetMasterVolume(Volume set)
    {
        auto audioSystem = Manager()->FindSystem<AudioSystem>();
        if (audioSystem->Get()->SetMasterVolume(set))
            masterVolume = set;
    }

    Volume MasterSoundSystem::GetMasterVolume()
    {
        return masterVolume;
    }

    void MasterSoundSystem::InitializeImpl()
    {
        auto initialization = Manager()->FindSystem<InitializationFileSystem>();
        auto entry = initialization->sound.CreateEntry<Volume>("Master Volume", [this]()
        {
            return this->GetMasterVolume();
        });
        entry->onLoaded.Subscribe([this](Volume value) {
            this->SetMasterVolume(value);
        });
    }
}

namespace Inscription
{
    void Scribe<::Atmos::MasterSoundSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}