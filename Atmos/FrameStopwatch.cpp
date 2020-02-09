#include "FrameStopwatch.h"

#include "StartStopwatch.h"
#include "CalculateStopwatch.h"

namespace Atmos::Time
{
    FrameStopwatch::FrameStopwatch(Init init) :
        OpenTypedRelic(init),
        timeInformation(init.owner)
    {
        core = Create<Core>(
            [this]()
            {
                return CurrentTime();
            });
    }

    Value FrameStopwatch::Start() const
    {
        return Owner().Do<StartStopwatch>(ID());
    }

    Value FrameStopwatch::Elapsed() const
    {
        return Owner().Do<CalculateStopwatch>(ID());
    }

    Value FrameStopwatch::CurrentTime() const
    {
        return timeInformation->totalElapsed;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Time::FrameStopwatch, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {}
}