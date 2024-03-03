#pragma once

#include "ObjectSystem.h"

#include "Volume.h"

namespace Atmos::Audio
{
    class MasterSoundSystem : public ObjectSystem
    {
    public:
        MasterSoundSystem(ObjectManager& manager);

        // Value between 0 and 100
        void SetMasterVolume(Volume set);
        Volume GetMasterVolume();
    protected:
        void InitializeImpl() override;
    private:
        Volume masterVolume;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::MasterSoundSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Audio::MasterSoundSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}