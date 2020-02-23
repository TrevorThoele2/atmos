#include "FrameStartCurator.h"

namespace Atmos
{
    FrameStartCurator::FrameStartCurator(Init init) :
        Curator(init), timeInformation(init.owner),
        debugIdleProfiler(
            [](Debug::Statistics& statistics) -> Arca::Index<Time::RealStopwatch>&
            {
                return statistics.profilers.idle;
            },
            init.owner)
    {}

    void FrameStartCurator::Work()
    {
        auto timeInformationData = MutablePointer(timeInformation);
        timeInformationData->frameStartTime = timeInformationData->stopwatch->Start();

        debugIdleProfiler.Calculate();
    }
}
