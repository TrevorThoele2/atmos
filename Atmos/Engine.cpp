#include "Engine.h"

#include "TypeRegistration.h"

#include "LoadAssetsByZipUserContext.h"

namespace Atmos
{
    Engine::~Engine() = default;

    void Engine::UseField(World::Field&& field, std::vector<Property>&& worldProperties, const File::Path& assetsFilePath)
    {
        managers.world->UseField(std::move(field), std::move(worldProperties));
        ChangeField(0, assetsFilePath);
    }

    void Engine::LoadWorld(const File::Path& filePath, const File::Path& assetsFilePath)
    {
        managers.world->UseWorld(filePath);
        ChangeField(0, assetsFilePath);
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

    Engine::Engine(InitializationProperties&& initializationProperties, Logging::Logger& logger) : logger(&logger)
    {
        managers = Managers
        {
            std::move(initializationProperties.system),
            std::move(initializationProperties.assetResourceManager),
            std::move(initializationProperties.window),
            std::move(initializationProperties.audioManager),
            std::move(initializationProperties.inputManager),
            std::move(initializationProperties.graphicsManager),
            std::move(initializationProperties.textManager),
            std::move(initializationProperties.scriptManager),
            std::move(initializationProperties.worldManager)
        };

        managers.window->ChangeSize(Spatial::Size2D{ 1024, 768 });
        managers.window->CenterOnScreen();

        execution = std::make_unique<Execution>(*managers.world, *managers.window);
    }

    void Engine::ChangeField(World::FieldID fieldID, const File::Path& assetsFilePath)
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

        auto reliquary = origin.Actualize();

        managers.world->Request(fieldID);

        auto loadAssetsUserContext = Inscription::LoadAssetsByZipUserContext(assetsFilePath, *logger);

        managers.world->LockIn(std::move(reliquary), loadAssetsUserContext);
    }
}