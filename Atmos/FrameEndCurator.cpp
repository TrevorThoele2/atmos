#include "FrameEndCurator.h"

#include "StartStopwatch.h"
#include "CalculateStopwatch.h"

namespace Atmos
{
    FrameEndCurator::FrameEndCurator(Init init) :
        Curator(init),
        fpsTimer(Owner().Do<Arca::Create<Time::RealStopwatch>>()),
        timeInformation(init.owner),
        timeSettings(init.owner),
        debugStatistics(init.owner)
    {}

    void FrameEndCurator::Work()
    {
        auto timeInformationData = Data(timeInformation);

        static unsigned int count = 0;
        if (fpsTimer->Elapsed().Get() >= Time::Value::Number(timeSettings->fpsLimit))
        {
            Owner().Do<Time::StartStopwatch>(fpsTimer.ID());

            timeInformationData->fps = count;
            count = 0;
        }

        ++count;

        timeInformationData->frameEndTime =
            Owner().Do<Time::CalculateStopwatch>(timeInformationData->stopwatch.ID());
        timeInformationData->lastFrameElapsed =
            timeInformationData->frameEndTime - timeInformationData->frameStartTime;
        timeInformationData->totalElapsed += timeInformationData->lastFrameElapsed;

        Owner().Do<Time::StartStopwatch>(debugStatistics->profilers.idle.ID());
    }
}