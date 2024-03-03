#include "Engine.h"

#include "TypeRegistration.h"
#include "EngineNotSetup.h"

#include "WindowProvider.h"
#include "UniqueProvider.h"

namespace Atmos
{
    Engine::~Engine() = default;

    void Engine::Setup()
    {
        if (IsSetup())
            return;

        Arca::ReliquaryOrigin origin;
        RegisterGlobalTypes(origin);
        globalReliquary = origin.Actualize();

        ProvideInitializationProperties(CreateInitializationProperties(*globalReliquary));

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

    void Engine::ProvideInitializationProperties(InitializationProperties&& properties)
    {
        Window::window.Setup(std::move(properties.window));
        Arca::GlobalPtr<UniqueProvider<Input::Manager>>(*globalReliquary)->Change(
            std::move(properties.inputManager));
        Arca::GlobalPtr<UniqueProvider<Render::GraphicsManager>>(*globalReliquary)->Change(
            std::move(properties.graphicsManager));
        Arca::GlobalPtr<UniqueProvider<Audio::AudioManager>>(*globalReliquary)->Change(
            std::move(properties.audioManager));
    }
}