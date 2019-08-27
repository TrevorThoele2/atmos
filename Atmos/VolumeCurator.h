#pragma once

#include <Arca/Curator.h>

#include "VolumeInformation.h"
#include "ChangeMasterVolume.h"
#include "InitializationInformation.h"

#include "Serialization.h"

namespace Atmos::Audio
{
    class VolumeCurator final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
        void WorkImplementation(Stage& stage) override;
    private:
        VolumeInformation* volumeInformation = nullptr;
        Initialization::Information* initializationInformation = nullptr;

        Arca::Batch<ChangeMasterVolume> changeMasterVolume;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::VolumeCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Audio::VolumeCurator, BinaryArchive>
    {};
}