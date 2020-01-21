#include "VolumeCurator.h"

#include <Arca/Reliquary.h>

namespace Atmos::Audio
{
    void VolumeCurator::InitializeImplementation()
    {
        volumeInformation = Arca::GlobalIndex<VolumeInformation>(Owner());
        initializationInformation = Arca::GlobalIndex<Initialization::Information>(Owner());
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