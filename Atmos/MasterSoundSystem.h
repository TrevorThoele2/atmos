#pragma once

#include "ObjectSystem.h"

#include "Volume.h"

namespace Atmos
{
    class MasterSoundSystem : public ObjectSystem
    {
    public:
        MasterSoundSystem(ObjectManager& manager);
        MasterSoundSystem(const ::Inscription::Table<MasterSoundSystem>& table);

        // Value between 0 and 100
        void SetMasterVolume(Volume set);
        Volume GetMasterVolume();
    private:
        Volume masterVolume;
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::MasterSoundSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}