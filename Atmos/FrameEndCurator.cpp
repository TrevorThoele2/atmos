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

        const auto timeSettings = Owner().Find<Settings>();
        
        if (framesPerSecondStopwatch.Elapsed() >= Time::Seconds(1))
        {
            framesPerSecondStopwatch.Restart();

            information->framesPerSecond = frameCount;
            frameCount = 0;
        }

        ++frameCount;
        
        information->lastElapsed = information->profilers.frame.Elapsed();
        information->totalElapsed += information->lastElapsed;

        information->profilers.idle = Time::CreateRealStopwatch();

        MutablePointer().Of<Diagnostics::Statistics>()->frame.NewTime(
            Diagnostics::CalculateStopwatch(information->profilers.frame));
    }
}