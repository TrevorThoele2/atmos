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
        SoundSystem(const ::Inscription::Table<SoundSystem>& table);

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
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}