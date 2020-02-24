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
            MutablePointer())
    {}

    void FrameStartCurator::Work()
    {
        auto timeInformationData = MutablePointer().Of(timeInformation);
        timeInformationData->frameStartTime = timeInformationData->stopwatch->Start();

        debugIdleProfiler.Calculate();
    }
}
