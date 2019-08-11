#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "Sound.h"

namespace Atmos::Audio
{
    class SoundSystem : public ObjectSystem
    {
    public:
        SoundSystem(ObjectManager& manager);

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
    template<>
    class Scribe<::Atmos::Audio::SoundSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Audio::SoundSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}