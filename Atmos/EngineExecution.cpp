#include "EngineExecution.h"

#include "Work.h"
#include "FocusLost.h"
#include "FocusRegained.h"

#include "WorldManager.h"

#include "WindowBase.h"
#include "TimePoint.h"

namespace Atmos
{
    EngineExecution::EngineExecution(World::Manager& worldManager, Window::WindowBase& window) :
        window(&window), worldManager(&worldManager)
    {}

    void EngineExecution::Start()
    {
        while (StartFrame())
        {
            if (IsCurrentlyFocused())
            {
                if (worldManager->CurrentField())
                    worldManager->CurrentField()->Reliquary().Do(Work{});
            }
            else
                window->Suspend(std::chrono::duration_cast<Time::Seconds>(Time::Milliseconds(1)));
        }
    }

    bool EngineExecution::StartFrame()
    {
        if (IsCurrentlyFocused() != wasFocusedLastFrame)
        {
            auto currentField = worldManager->CurrentField();

            if (currentField)
            {
                wasFocusedLastFrame
                    ? currentField->Reliquary().Raise(FocusLost())
                    : currentField->Reliquary().Raise(FocusRegained());
            }
            wasFocusedLastFrame = !wasFocusedLastFrame;
        }

        return window->OnStartFrame();
    }

    bool EngineExecution::IsCurrentlyFocused() const
    {
        return window->IsCurrentlyFocused();
    }
}
