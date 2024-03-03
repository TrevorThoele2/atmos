#include "FrameStartCurator.h"

#include "StartStopwatch.h"
#include "CalculateStopwatch.h"

namespace Atmos
{
    FrameStartCurator::FrameStartCurator(Init init) :
        Curator(init), timeInformation(init.owner), debugStatistics(init.owner)
    {}

    void FrameStartCurator::Work()
    {
        auto timeInformationData = Data(timeInformation);
        timeInformationData->frameStartTime = Owner().Do<Time::StartStopwatch>(timeInformationData->ID());

        Owner().Do<Time::CalculateStopwatch>(debugStatistics->profilers.idle.ID());
    }
}
