
#include "Engine.h"

namespace Atmos
{
    Engine::~Engine()
    {}

    void Engine::Setup()
    {
        registration.PushGlobalsTo(globalObjectManager);
    }

    void Engine::LoadWorld(const FilePath& filePath)
    {
        worldManager.UseWorld(filePath);
    }

    void Engine::StartExecution()
    {
        execution.Start();
    }

    void Engine::Exit()
    {
        DoExit();
    }

    Engine::Engine() : execution(globalObjectManager, worldManager), worldManager(globalObjectManager, registration)
    {}
}