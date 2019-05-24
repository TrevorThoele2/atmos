#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "Sound.h"

namespace Atmos
{
    class SoundSystem : public ObjectSystem
    {
    public:
        SoundSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(SoundSystem);

        void ResumeAll();
        void PauseAll();
    private:
        typedef ObjectBatch<Sound> SoundBatch;
        SoundBatch sounds;
    private:
        void OnCameraMoved();
        void OnEngineFocusLost();
        void OnEngineFocusRegained();
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::SoundSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}