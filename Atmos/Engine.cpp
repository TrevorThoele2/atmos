#include "Engine.h"

#include "TypeRegistration.h"
#include "EngineNotSetup.h"

#include "LoadAssetsByZipUserContext.h"

namespace Atmos
{
    Engine::~Engine() = default;

    void Engine::Setup()
    {
        if (IsSetup())
            return;

        auto initializationProperties = CreateInitializationProperties(*logger);

        managers = Managers
        {
            std::move(initializationProperties.imageAssetManager),
            std::move(initializationProperties.window),
            std::move(initializationProperties.audioManager),
            std::move(initializationProperties.inputManager),
            std::move(initializationProperties.graphicsManager),
            std::move(initializationProperties.scriptManager)
        };

        managers.window->ChangeSize(Spatial::ScreenSize{ 1024, 768 });
        managers.window->CenterOnScreen();

        World::WorldManager worldManager;

        executionContext = std::make_unique<ExecutionContext>(std::move(worldManager), *managers.window);
    }

    void Engine::UseField(World::Field&& field, const File::Path& assetsFilePath)
    {
        SetupRequired();

        executionContext->worldManager.UseField(std::move(field));
        ChangeField(0, assetsFilePath);
    }

    void Engine::LoadWorld(const File::Path& filePath, const File::Path& assetsFilePath)
    {
        SetupRequired();

        executionContext->worldManager.UseWorld(filePath);
        ChangeField(0, assetsFilePath);
    }

    World::Field* Engine::CurrentField()
    {
        return executionContext->worldManager.CurrentField();
    }

    void Engine::StartExecution()
    {
        SetupRequired();

        executionContext->execution.Start();
    }

    void Engine::Exit()
    {
        DoExit();
    }

    Logging::Logger& Engine::Logger()
    {
        return *logger;
    }

    Engine::Engine(Logging::Logger& logger) : logger(&logger)
    {}

    bool Engine::IsSetup() const
    {
        return executionContext != nullptr;
    }

    void Engine::SetupRequired() const
    {
        if (!IsSetup())
            throw EngineNotSetup();
    }

    Engine::ExecutionContext::ExecutionContext(World::WorldManager&& worldManager, Window::WindowBase& window) :
        execution(this->worldManager, window),
        worldManager(std::move(worldManager))
    {}

    void Engine::ChangeField(World::FieldID fieldID, const File::Path& assetsFilePath)
    {
        Arca::ReliquaryOrigin origin;

        RegisterFieldTypes(
            origin,
            *managers.imageAssetManager,
            *managers.audio,
            *managers.input,
            *managers.graphics,
            *managers.scripts,
            Spatial::ScreenSize{ 1024, 768 },
            *managers.window,
            *logger);
        RegisterFieldStages(origin);

        auto reliquary = origin.Actualize();

        executionContext->worldManager.Request(fieldID);

        auto loadAssetsUserContext = Inscription::LoadAssetsByZipUserContext(assetsFilePath, *logger);

        executionContext->worldManager.LockIn(std::move(reliquary), loadAssetsUserContext);
    }
}