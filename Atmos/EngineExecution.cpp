#include "EngineExecution.h"

#include "Work.h"
#include "FocusLost.h"
#include "FocusRegained.h"

#include "WorldManager.h"

#include "WindowBase.h"
#include "TimePoint.h"

namespace Atmos
{
    EngineExecution::EngineExecution(World::WorldManager& worldManager, Window::WindowBase& window) :
        window(&window), worldManager(&worldManager)
    {}

    void EngineExecution::Start()
    {
        while (StartFrame())
        {
            if (IsCurrentlyFocused())
            {
                if (isFocusLost)
                    OnFocusRegain();

                if (worldManager->CurrentField())
                    worldManager->CurrentField()->Reliquary().Do(Work{});
            }
            else
            {
                if (!isFocusLost)
                    OnFocusLost();

                window->Suspend(std::chrono::duration_cast<Time::Seconds>(Time::Milliseconds(1)));
            }
        }
    }

    bool EngineExecution::StartFrame()
    {
        if (IsCurrentlyFocused() != wasFocusedLastPass)
        {
            auto currentField = worldManager->CurrentField();

            if (currentField)
            {
                wasFocusedLastPass
                    ? currentField->Reliquary().Raise(FocusLost())
                    : currentField->Reliquary().Raise(FocusRegained());
            }
            wasFocusedLastPass = !wasFocusedLastPass;
        }

        return window->OnStartFrame();
    }

    bool EngineExecution::IsCurrentlyFocused() const
    {
        return window->IsCurrentlyFocused();
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
