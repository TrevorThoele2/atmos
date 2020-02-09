#include "VolumeCurator.h"

#include <Arca/Reliquary.h>

namespace Atmos::Audio
{
    VolumeCurator::VolumeCurator(Init init) :
        Curator(init), volumeInformation(init.owner), initializationInformation(init.owner)
    {}

    void VolumeCurator::Handle(const ChangeMasterVolume& command)
    {
        Data(volumeInformation)->master = command.to;
        Data(initializationInformation)->sound.masterVolume = command.to;
    }
}