#include "IdleCurator.h"

#include "WindowProvider.h"

namespace Atmos::Time
{
    IdleCurator::IdleCurator(Init init) : Curator(init)
    {}

    void IdleCurator::Work()
    {
        while (!IsDone())
            Window::window->Suspend(Time::Duration<>(0));
    }

    bool IdleCurator::IsDone() const
    {
        if (timeSettings->fpsLimit == 0 || timeSettings->verticalSync)
            return true;

        return timeInformation->stopwatch->Elapsed() >= Milliseconds(timeSettings->fpsLimit);
    }
}