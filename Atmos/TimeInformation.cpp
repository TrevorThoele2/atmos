#include "TimeInformation.h"

namespace Atmos::Time
{
    void Information::PostConstruct()
    {
        stopwatch = Stopwatch(Owner().Create<RealStopwatch>());
    }
}