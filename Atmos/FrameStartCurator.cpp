#include "FrameStartCurator.h"

#include "FrameInformation.h"

#include "DiagnosticsStatistics.h"

namespace Atmos::Frame
{
    StartCurator::StartCurator(Init init) : Curator(init)
    {}

    void StartCurator::Handle(const Work&)
    {
        auto frameInformation = MutablePointer().Of<Information>();
        frameInformation->profilers.frame.Restart();
        
        MutablePointer().Of<Diagnostics::Statistics>()->idle.NewTime(
            Diagnostics::CalculateStopwatch(frameInformation->profilers.idle));
    }
}
