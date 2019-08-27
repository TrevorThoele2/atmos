#include "Engine.h"

#include "TypeRegistration.h"
#include "EngineNotSetup.h"

#include "WindowProvider.h"
#include "InputManagerProvider.h"
#include "GraphicsManagerProvider.h"
#include "AudioManagerProvider.h"

namespace Atmos
{
    Engine::~Engine() = default;

    void Engine::Setup()
    {
        if (IsSetup())
            return;

        Arca::ReliquaryOrigin origin;
        RegisterTypes(origin);
        reliquary = origin.Actualize();

        ProvideInitializationProperties(CreateInitializationProperties(*reliquary));

        World::WorldManager worldManager(*reliquary);

        executionContext = std::make_unique<ExecutionContext>(*this, std::move(worldManager));
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
        execution(EngineExecution(*owner.reliquary, this->worldManager)),
        worldManager(std::move(worldManager))
    {}

    void Engine::ProvideInitializationProperties(InitializationProperties&& properties)
    {
        Window::window.Setup(std::move(properties.window));
        reliquary->Find<Input::ManagerProvider>()->Change(std::move(properties.inputManager));
        reliquary->Find<Render::GraphicsManagerProvider>()->Change(std::move(properties.graphicsManager));
        reliquary->Find<Audio::AudioManagerProvider>()->Change(std::move(properties.audioManager));
    }
}