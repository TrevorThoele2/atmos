#include "VolumeCurator.h"

#include <Arca/Reliquary.h>

namespace Atmos::Audio
{
    void VolumeCurator::InitializeImplementation()
    {
        volumeInformation = Owner().Find<VolumeInformation>();
        initializationInformation = Owner().Find<Initialization::Information>();
    }

    void VolumeCurator::WorkImplementation(Stage& stage)
    {
        for (auto& loop : changeMasterVolume)
        {
            volumeInformation->master = loop.to;
            initializationInformation->sound.masterVolume = loop.to;
        }
    }
}