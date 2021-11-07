#include "FrameStartCurator.h"

#include "FrameInformation.h"

#include "DiagnosticsStatistics.h"

namespace Atmos::Frame
{
    StartCurator::StartCurator(Init init) : Curator(init)
    {}

    void StartCurator::Handle(const Work&)
    {
        const auto information = MutablePointer().Of<Information>();

        information->lastElapsed = information->profilers.frame.Elapsed();
        information->totalElapsed += information->lastElapsed;

        information->profilers.frame.Restart();
        
        MutablePointer().Of<Diagnostics::Statistics>()->idle.NewTime(information->profilers.idle);
    }
}
