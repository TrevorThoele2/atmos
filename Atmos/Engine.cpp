#include "Engine.h"

#include "TypeRegistration.h"

namespace Atmos
{
    Engine::~Engine()
    {
        managers.scripting.reset();
        managers.world.reset();
        managers.graphics.reset();
    }

    void Engine::UseField(World::Field&& field, std::vector<Property>&& worldProperties, const File::Path& assetsFilePath)
    {
        this->assetsFilePath = assetsFilePath;
        managers.world->UseField(std::move(field), std::move(worldProperties));
        ChangeField(0);
    }

    void Engine::LoadWorld(const File::Path& filePath, const File::Path& assetsFilePath)
    {
        this->assetsFilePath = assetsFilePath;
        managers.world->UseWorld(filePath);
        ChangeField(0);
    }

    World::Field* Engine::CurrentField()
    {
        return managers.world->CurrentField();
    }

    void Engine::StartExecution()
    {
        execution->Start();
    }

    Logging::Logger& Engine::Logger()
    {
        return *logger;
    }

    World::Manager* Engine::WorldManager() const
    {
        return managers.world.get();
    }
   
    void Engine::ChangeField(World::FieldID fieldID)
    {
        managers.world->Request(fieldID);
        managers.world->LockIn();
    }

    World::FieldInitialization Engine::CreateFieldInitialization() const
    {
        return
        {
            managers.assetResource.get(),
            managers.audio.get(),
            managers.input.get(),
            managers.graphics.get(),
            managers.text.get(),
            managers.scripting.get(),
            managers.window.get(),
            logger,
            assetsFilePath
        };
    }
}