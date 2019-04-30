
#include "MasterSoundSystem.h"

#include "ObjectManager.h"
#include "AudioSystem.h"
#include "InitializationFileSystem.h"

namespace Atmos
{
    MasterSoundSystem::MasterSoundSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    MasterSoundSystem::MasterSoundSystem(const ::Inscription::Table<MasterSoundSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
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
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::MasterSoundSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::MasterSoundSystem, "MasterSoundSystem");
}