
#include "SoundSystem.h"

#include "ObjectManager.h"
#include "CameraSystem.h"
#include "EngineSystem.h"

namespace Atmos
{
    SoundSystem::SoundSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        sounds = Manager()->Batch<Sound>();

        auto cameraSystem = Manager()->FindSystem<CameraSystem>();
        cameraSystem->eventMoved.Subscribe(&SoundSystem::OnCameraMoved, *this);

        auto engineSystem = Manager()->FindSystem<EngineSystem>();
        engineSystem->onFocusLost.Subscribe(&SoundSystem::OnEngineFocusLost, *this);
        engineSystem->onFocusRegained.Subscribe(&SoundSystem::OnEngineFocusRegained, *this);
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

    void SoundSystem::OnEngineFocusLost()
    {
        PauseAll();
    }

    void SoundSystem::OnEngineFocusRegained()
    {
        ResumeAll();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::SoundSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}