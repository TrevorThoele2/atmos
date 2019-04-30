
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

    SoundSystem::SoundSystem(const ::Inscription::Table<SoundSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

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
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::SoundSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::SoundSystem, "SoundSystem");
}