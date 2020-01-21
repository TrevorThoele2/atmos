#include "EngineExecution.h"

#include <Arca/Reliquary.h>
#include "WindowProvider.h"
#include "TimeCurator.h"
#include "DebugStatistics.h"
#include "FocusLost.h"
#include "FocusRegained.h"

#include "WorldManager.h"

namespace Atmos
{
    EngineExecution::EngineExecution(Arca::Reliquary& globalReliquary, World::WorldManager& worldManager) :
        globalReliquary(&globalReliquary), worldManager(&worldManager)
    {}

    void EngineExecution::Start()
    {
        auto& timeCurator = globalReliquary->Find<Time::TimeCurator>();
        auto debugStatistics = Arca::GlobalIndex<Debug::Statistics>(*globalReliquary);

        while (StartFrame())
        {
            if (IsCurrentlyFocused())
            {
                if (isFocusLost)
                    OnFocusRegain();

                debugStatistics->profilers.idle.Calculate();

                worldManager->Work();

                debugStatistics->profilers.idle.Start();

                while (timeCurator.DoIdle())
                    Window::window->Suspend(Time::Value(FixedPoint64(0), Time::Epoch::Milliseconds));
            }
            else
            {
                if (!isFocusLost)
                    OnFocusLost();

                Window::window->Suspend(Time::Value(FixedPoint64(1), Time::Epoch::Milliseconds));
            }
        }
    }

    bool EngineExecution::StartFrame()
    {
        if (IsCurrentlyFocused() != wasFocusedLastPass)
        {
            wasFocusedLastPass ?
                globalReliquary->Raise<FocusLost>() :
                globalReliquary->Raise<FocusRegained>();
            wasFocusedLastPass = !wasFocusedLastPass;
        }

        return Window::window->OnStartFrame();
    }

    bool EngineExecution::IsCurrentlyFocused() const
    {
        return Window::window->IsCurrentlyFocused();
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
