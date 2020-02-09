#include "TimeInformation.h"

#include "StopwatchStatistics.h"

namespace Atmos::Time
{
    Information::Information(Init init)
        : ClosedTypedRelic(init), stopwatch(init.owner.Do<Arca::Create<RealStopwatch>>())
    {
        stopwatch->Create<StopwatchStatistics>();
    }
}