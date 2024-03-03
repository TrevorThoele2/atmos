#include "SoundCurator.h"

#include <Arca/Reliquary.h>

namespace Atmos::Audio
{
    SoundCurator::SoundCurator(Init init) :
        Curator(init), batch(init.owner.Batch<Sound>())
    {}

    void SoundCurator::ResumeAll()
    {

    }

    void SoundCurator::PauseAll()
    {

    }

    void SoundCurator::OnCameraMoved()
    {

    }

    void SoundCurator::OnEngineFocusLost()
    {
        PauseAll();
    }

    void SoundCurator::OnEngineFocusRegained()
    {
        ResumeAll();
    }
}