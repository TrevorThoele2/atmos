
#include "TimeHandler.h"
#include "Fps.h"

namespace Atmos
{
    void TimeHandler::OnFrameEnd()
    {
        lastElapsed = (FpsHandler::GetFrameEnd() - FpsHandler::GetFrameStart());
        totalElapsed += lastElapsed;
    }

    TimeValue TimeHandler::GetTotalElapsed() const
    {
        return totalElapsed;
    }

    TimeValue TimeHandler::GetLastElapsed() const
    {
        return lastElapsed;
    }

    TimeValue TimeHandler::CurrentTime() const
    {
        return TimeValue(clock.now().time_since_epoch().count(), TimeValueEpoch::NANOSECONDS).ConvertValue(TimeValueEpoch::SECONDS);
    }
}