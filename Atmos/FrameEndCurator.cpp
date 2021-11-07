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
        const auto information = MutablePointer().Of<Information>();

        const auto timeSettings = Owner().Find<Settings>();

        CalculateFPS(*information);
        
        information->profilers.idle = Time::CreateRealStopwatch();

        MutablePointer().Of<Diagnostics::Statistics>()->frame.NewTime(information->profilers.frame);
    }

    void EndCurator::CalculateFPS(Information& information)
    {
        if (framesPerSecondStopwatch.Elapsed() >= Time::Seconds(1))
        {
            framesPerSecondStopwatch.Restart();

            information.framesPerSecond = frameCount;
            frameCount = 0;
        }

        ++frameCount;
    }
}