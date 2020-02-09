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

        Arca::ReliquaryOrigin origin;
        RegisterGlobalTypes(
            origin,
            std::move(initializationProperties.inputManager),
            std::move(initializationProperties.graphicsManager),
            std::move(initializationProperties.audioManager));
        globalReliquary = origin.Actualize();

        World::WorldManager worldManager(*globalReliquary);

        executionContext = std::make_unique<ExecutionContext>(*this, std::move(worldManager));
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

    void Engine::StartExecution()
    {
        SetupRequired();

        executionContext->execution.Start();
    }

    void Engine::Exit()
    {
        DoExit();
    }

    Arca::Reliquary* Engine::GlobalReliquary()
    {
        return globalReliquary.get();
    }

    const Arca::Reliquary* Engine::GlobalReliquary() const
    {
        return globalReliquary.get();
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

    Engine::ExecutionContext::ExecutionContext(Engine& owner, World::WorldManager&& worldManager) :
        execution(EngineExecution(*owner.globalReliquary, this->worldManager)),
        worldManager(std::move(worldManager))
    {}
}