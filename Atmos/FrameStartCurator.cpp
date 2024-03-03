#include "FrameStartCurator.h"

#include "FrameInformation.h"

namespace Atmos::Frame
{
    StartCurator::StartCurator(Init init) :
        Curator(init),
        debugIdleProfiler(
            [](Debug::Statistics& statistics) -> Time::Stopwatch&
            {
                return statistics.profilers.idle;
            },
            MutablePointer())
    {}

    void StartCurator::Work()
    {
        auto mutableInformation = MutablePointer().Of<Information>();
        mutableInformation->startTime = mutableInformation->stopwatch.Start();

        debugIdleProfiler.Calculate();
    }
}
