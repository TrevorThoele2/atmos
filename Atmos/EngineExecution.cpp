#include "EngineExecution.h"

#include <Arca/Reliquary.h>
#include "WindowProvider.h"
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
        while (StartFrame())
        {
            if (IsCurrentlyFocused())
            {
                if (isFocusLost)
                    OnFocusRegain();

                worldManager->Work();
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
