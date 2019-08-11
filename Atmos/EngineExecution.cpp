#include "EngineExecution.h"

#include "ObjectManager.h"
#include "EngineSystem.h"
#include "WindowSystem.h"
#include "FpsSystem.h"
#include "DebugStatisticsSystem.h"

#include "WorldManager.h"

namespace Atmos
{
    EngineExecution::EngineExecution(ObjectManager& globalObjectManager, World::WorldManager& worldManager) :
        globalObjectManager(&globalObjectManager), worldManager(&worldManager),
        engineSystem(globalObjectManager.FindSystem<EngineSystem>()), isFocusLost(false)
    {}

    void EngineExecution::Start()
    {
        auto windowSystem = globalObjectManager->FindSystem<Window::WindowSystem>();
        auto fpsSystem = globalObjectManager->FindSystem<Time::FpsSystem>();
        auto debugStatistics = globalObjectManager->FindSystem<DebugStatisticsSystem>();

        while (StartFrame(windowSystem))
        {
            if (IsCurrentlyFocused(windowSystem)) // Check for focus on the window 
            {
                // Handle focus regained
                if (isFocusLost)
                    OnFocusRegain();

                debugStatistics->idleProfiler.Calculate();

                globalObjectManager->Work();
                worldManager->Work();

                debugStatistics->idleProfiler.Start();

                while (fpsSystem->DoIdle())
                    windowSystem->Get()->Suspend(Time::Value(FixedPoint64(0), Time::Epoch::MILLISECONDS));
            }
            else // Window is not in focus
            {
                if (!isFocusLost)
                    OnFocusLost();

                windowSystem->Get()->Suspend(Time::Value(FixedPoint64(1), Time::Epoch::MILLISECONDS));
            }
        }
    }

    bool EngineExecution::StartFrame(Window::WindowSystem* windowSystem)
    {
        // Check for and emit focus events
        if (IsCurrentlyFocused(windowSystem) != wasFocusedLastPass)
        {
            (wasFocusedLastPass) ? engineSystem->onFocusLost() : engineSystem->onFocusRegained();
            wasFocusedLastPass = !wasFocusedLastPass;
        }

        return windowSystem->Get()->OnStartFrame();
    }

    bool EngineExecution::IsCurrentlyFocused(Window::WindowSystem* windowSystem) const
    {
        return windowSystem->Get()->IsCurrentlyFocused();
    }

    void EngineExecution::OnFocusLost()
    {
        isFocusLost = true;
    }

    void EngineExecution::OnFocusRegain()
    {
        isFocusLost = false;
    }
}