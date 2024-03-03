#include "SoundCurator.h"

#include <Arca/Reliquary.h>

namespace Atmos::Audio
{
    void SoundCurator::ResumeAll()
    {

    }

    void SoundCurator::PauseAll()
    {

    }

    void SoundCurator::InitializeImplementation()
    {
        batch = Owner().Batch<Sound>();
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

namespace Arca
{
    const TypeName Traits<Atmos::Audio::SoundCurator>::typeName;
}