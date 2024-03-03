#include "FrameEndCurator.h"

namespace Atmos
{
    FrameEndCurator::FrameEndCurator(Init init) :
        Curator(init),
        fpsTimer(Owner().Do<Arca::Create<Time::RealStopwatch>>()),
        timeInformation(init.owner),
        timeSettings(init.owner),
        debugIdleProfiler(
            [](Debug::Statistics& statistics) -> Arca::Index<Time::RealStopwatch>&
            {
                return statistics.profilers.idle;
            },
            init.owner)
    {}

    void FrameEndCurator::Work()
    {
        auto timeInformationData = MutablePointer(timeInformation);

        static unsigned int count = 0;
        if (fpsTimer->Elapsed().Get() >= Time::Value::Number(timeSettings->fpsLimit))
        {
            fpsTimer->Start();

            timeInformationData->fps = count;
            count = 0;
        }

        ++count;

        timeInformationData->frameEndTime = timeInformationData->stopwatch->Elapsed();
        timeInformationData->lastFrameElapsed =
            timeInformationData->frameEndTime - timeInformationData->frameStartTime;
        timeInformationData->totalElapsed += timeInformationData->lastFrameElapsed;

        debugIdleProfiler.Start();
    }
}