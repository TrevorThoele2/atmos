#pragma once

#include "ObjectSystem.h"

#include "Volume.h"

namespace Atmos
{
    class MasterSoundSystem : public ObjectSystem
    {
    public:
        MasterSoundSystem(ObjectManager& manager);

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
    template<>
    class Scribe<::Atmos::MasterSoundSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::MasterSoundSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}