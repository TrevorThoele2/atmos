#include "EngineExecution.h"

#include "WindowProvider.h"
#include "FocusLost.h"
#include "FocusRegained.h"

#include "WorldManager.h"

namespace Atmos
{
    EngineExecution::EngineExecution(World::WorldManager& worldManager) : worldManager(&worldManager)
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
                    worldManager->CurrentField()->Reliquary().Work();
            }
            else
            {
                if (!isFocusLost)
                    OnFocusLost();

                Window::window->Suspend(std::chrono::duration_cast<Time::Seconds>(Time::Milliseconds(1)));
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
