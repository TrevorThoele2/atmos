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
    class Scribe<::Atmos::SoundSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::SoundSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}