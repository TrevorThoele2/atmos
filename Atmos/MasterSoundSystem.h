#pragma once

#include "ObjectSystem.h"

#include "Volume.h"

namespace Atmos
{
    class MasterSoundSystem : public ObjectSystem
    {
    public:
        MasterSoundSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(MasterSoundSystem);

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
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}