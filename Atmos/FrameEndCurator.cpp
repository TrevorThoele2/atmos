#include "FrameEndCurator.h"

#include "FrameSettings.h"

#include "CreateStopwatch.h"

#include "DiagnosticsStatistics.h"

namespace Atmos::Frame
{
    EndCurator::EndCurator(Init init) : Curator(init), framesPerSecondStopwatch(Time::CreateRealStopwatch())
    {}

    void EndCurator::Handle(const Work&)
    {
        auto information = MutablePointer().Of<Information>();

        const auto timeSettings = Arca::Index<Settings>(Owner());
        
        if (framesPerSecondStopwatch.Elapsed() >= Time::Milliseconds(timeSettings->framesPerSecondLimit))
        {
            framesPerSecondStopwatch.Restart();

            information->framesPerSecond = count;
            count = 0;
        }

        ++count;

        information->endTime = information->profilers.frame.CurrentTime();
        information->lastElapsed = information->endTime - information->startTime;
        information->totalElapsed += information->lastElapsed;

        information->profilers.idle = Time::CreateRealStopwatch();
    }
}