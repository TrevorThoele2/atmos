#include "Engine.h"

#include "TypeRegistration.h"
#include "EngineNotSetup.h"

#include "WindowProvider.h"

#include "LoadAssetsByZipUserContext.h"

namespace Atmos
{
    Engine::~Engine() = default;

    void Engine::Setup()
    {
        if (IsSetup())
            return;

        auto initializationProperties = CreateInitializationProperties();
        Window::window.Setup(std::move(initializationProperties.window));
        Window::window->ChangeSize(Spatial::ScreenSize{ 1024, 768 });
        Window::window->CenterOnScreen();

        managers = Managers
        {
            std::move(initializationProperties.audioManager),
            std::move(initializationProperties.inputManager),
            std::move(initializationProperties.graphicsManager)
        };

        World::WorldManager worldManager;

        executionContext = std::make_unique<ExecutionContext>(std::move(worldManager));
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

    bool Engine::IsSetup() const
    {
        return executionContext != nullptr;
    }

    void Engine::SetupRequired() const
    {
        if (!IsSetup())
            throw EngineNotSetup();
    }

    Engine::ExecutionContext::ExecutionContext(World::WorldManager&& worldManager) :
        execution(this->worldManager),
        worldManager(std::move(worldManager))
    {}

    void Engine::ChangeField(World::FieldID fieldID, const File::Path& assetsFilePath)
    {
        Arca::ReliquaryOrigin origin;

        RegisterFieldTypes(
            origin,
            *managers.audio,
            *managers.input,
            *managers.graphics,
            Spatial::ScreenSize{ 1024, 768 },
            Window::window->Handle());
        RegisterFieldStages(origin);

        auto reliquary = origin.Actualize();

        executionContext->worldManager.Request(fieldID);

        auto loadAssetsUserContext = Inscription::LoadAssetsByZipUserContext(assetsFilePath);

        executionContext->worldManager.LockIn(std::move(reliquary), loadAssetsUserContext);
    }
}