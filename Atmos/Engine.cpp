#include "Engine.h"

#include "TypeRegistration.h"

#include "RealWorldManager.h"
#include "LoadAssetsByZipUserContext.h"

namespace Atmos
{
    Engine::~Engine()
    {
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

    Engine::Engine(InitializationProperties&& initializationProperties, Logging::Logger& logger) :
        managers
        {
            std::move(initializationProperties.system),
            std::move(initializationProperties.assetResourceManager),
            std::move(initializationProperties.window),
            std::move(initializationProperties.audioManager),
            std::move(initializationProperties.inputManager),
            std::move(initializationProperties.graphicsManager),
            std::move(initializationProperties.textManager),
            std::move(initializationProperties.scriptManager),
            std::make_unique<World::RealManager>(
                [this]() { return CreateReliquary(); },
                [this]() { return CreateLoadAssetsUserContext(); })
        },
        logger(&logger)
    {
        managers.window->ChangeSize(Spatial::Size2D{ 1024, 768 });
        managers.window->CenterOnScreen();

        execution = std::make_unique<Execution>(*managers.world, *managers.window);
    }

    void Engine::ChangeField(World::FieldID fieldID)
    {
        managers.world->Request(fieldID);
        managers.world->LockIn();
    }

    std::unique_ptr<Arca::Reliquary> Engine::CreateReliquary()
    {
        Arca::ReliquaryOrigin origin;

        RegisterFieldTypes(
            origin,
            *managers.assetResourceManager,
            *managers.audio,
            *managers.input,
            *managers.graphics,
            *managers.text,
            *managers.scripts,
            *managers.world,
            Spatial::Size2D{ 1024, 768 },
            *managers.window,
            *logger);
        RegisterFieldStages(origin);

        return origin.Actualize();
    }

    std::unique_ptr<Inscription::LoadAssetsUserContext> Engine::CreateLoadAssetsUserContext()
    {
        return std::make_unique<Inscription::LoadAssetsByZipUserContext>(assetsFilePath, *logger);
    }
}