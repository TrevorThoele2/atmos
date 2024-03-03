#include "SoundSystem.h"

#include "ObjectManager.h"
#include "CameraSystem.h"
#include "EngineSystem.h"

namespace Atmos::Audio
{
    SoundSystem::SoundSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        sounds = Manager()->Batch<Sound>();

        auto cameraSystem = Manager()->FindSystem<Render::CameraSystem>();
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
    void Scribe<::Atmos::Audio::SoundSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}