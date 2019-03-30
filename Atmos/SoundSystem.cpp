
#include "SoundSystem.h"

#include "ObjectManager.h"

#include "GameEnvironment.h"

namespace Atmos
{
    SoundSystem::SoundSystem(ObjectManager& objectManager) : ObjectSystem(objectManager)
    {
        sounds = objectManager.Batch<nSound>();
        GameEnvironment::GetCamera().eventMoved.Subscribe(&SoundSystem::OnCameraMoved, *this);
    }

    void SoundSystem::ResumeAll()
    {

    }

    void SoundSystem::PauseAll()
    {

    }

    void SoundSystem::OnCameraMoved()
    {

    }
}