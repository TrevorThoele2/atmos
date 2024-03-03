#include "IdleCurator.h"

#include "WindowProvider.h"

namespace Atmos::Time
{
    IdleCurator::IdleCurator(Init init) : Curator(init)
    {}

    void IdleCurator::Work()
    {
        while (!IsDone())
            Window::window->Suspend(Time::Value(FixedPoint64(0), Time::Epoch::Milliseconds));
    }

    bool IdleCurator::IsDone() const
    {
        if (timeSettings->fpsLimit == 0 || timeSettings->verticalSync)
            return true;

        return timeInformation->stopwatch->Elapsed().Get() >= Time::Value::Number(timeSettings->fpsLimit);
    }
}