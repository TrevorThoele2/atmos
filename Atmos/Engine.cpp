#include "Engine.h"

#include "TypeRegistration.h"
#include "EngineNotSetup.h"

#include "WindowProvider.h"

namespace Atmos
{
    Engine::~Engine() = default;

    void Engine::Setup()
    {
        if (IsSetup())
            return;

        auto initializationProperties = CreateInitializationProperties();
        Window::window.Setup(std::move(initializationProperties.window));

        managers = Managers
        {
            std::move(initializationProperties.audioManager),
            std::move(initializationProperties.inputManager),
            std::move(initializationProperties.graphicsManager)
        };

        World::WorldManager worldManager({ *managers.audio, *managers.input, *managers.graphics });

        executionContext = std::make_unique<ExecutionContext>(std::move(worldManager));
    }

    void Engine::UseField(World::Field&& field)
    {
        SetupRequired();

        executionContext->worldManager.UseField(std::move(field));
        executionContext->worldManager.Request(0);
        executionContext->worldManager.LockIn();
    }

    void Engine::LoadWorld(const File::Path& filePath)
    {
        SetupRequired();

        executionContext->worldManager.UseWorld(filePath);
        executionContext->worldManager.Request(0);
        executionContext->worldManager.LockIn();
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
}