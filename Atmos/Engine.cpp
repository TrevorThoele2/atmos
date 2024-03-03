#include "Engine.h"

#include "EngineTypeRegistration.h"
#include "EngineNotSetup.h"

namespace Atmos
{
    Engine::~Engine()
    {}

    void Engine::Setup()
    {
        if (IsSetup())
            return;

        TypeRegistration typeRegistration;
        auto globalTypes = typeRegistration.CreateGroup();
        auto localTypes = typeRegistration.CreateGroup();
        auto infrastructureTypes = typeRegistration.CreateGroup();
        RegisterGlobalTypes(*globalTypes);
        RegisterLocalTypes(*localTypes);
        RegisterInfrastructureTypes(*infrastructureTypes);

        globalTypes->PushTo(globalObjectManager);

        WorldManager worldManager(globalObjectManager, localObjectManagerFactory);

        executionContext.reset(new ExecutionContext(*this, std::move(worldManager)));
    }

    void Engine::LoadWorld(const FilePath& filePath)
    {
        SetupRequired();

        executionContext->worldManager.UseWorld(filePath);
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

    Engine::Engine() : localObjectManagerFactory(*localTypes, *infrastructureTypes)
    {}

    bool Engine::IsSetup() const
    {
        return executionContext != nullptr;
    }

    void Engine::SetupRequired()
    {
        if (!IsSetup())
            throw EngineNotSetup();
    }

    Engine::ExecutionContext::ExecutionContext(Engine& owner, WorldManager&& worldManager) :
        execution(EngineExecution(owner.globalObjectManager, this->worldManager)),
        worldManager(std::move(worldManager))
    {}
}