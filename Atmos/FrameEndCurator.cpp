#include "FrameEndCurator.h"

#include "FrameSettings.h"

#include "CreateStopwatch.h"

namespace Atmos::Frame
{
    EndCurator::EndCurator(Init init) :
        Curator(init),
        framesPerSecondStopwatch(Time::CreateRealStopwatch()),
        debugIdleProfiler(
            [](Debug::Statistics& statistics) -> Time::Stopwatch&
            {
                return statistics.profilers.idle;
            },
            MutablePointer())
    {}

    void EndCurator::Handle(const Work&)
    {
        auto mutableInformation = MutablePointer().Of<Information>();

        const auto timeSettings = Arca::Index<Settings>(Owner());

        static unsigned int count = 0;
        if (framesPerSecondStopwatch.Elapsed() >= Time::Milliseconds(timeSettings->framesPerSecondLimit))
        {
            framesPerSecondStopwatch.Restart();

            mutableInformation->framesPerSecond = count;
            count = 0;
        }

        ++count;

        mutableInformation->endTime = mutableInformation->stopwatch.CurrentTime();
        mutableInformation->lastElapsed = mutableInformation->endTime - mutableInformation->startTime;
        mutableInformation->totalElapsed += mutableInformation->lastElapsed;

        debugIdleProfiler.Start();
    }
}