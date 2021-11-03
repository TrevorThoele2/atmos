#include "RenderEndCurator.h"

#include "CreateStopwatch.h"
#include "DiagnosticsStatistics.h"

namespace Atmos::Render
{
    EndCurator::EndCurator(Init init) : Curator(init)
    {}

    void EndCurator::Handle(const Work&)
    {
        auto frameInformation = MutablePointer().Of<Frame::Information>();
        
        MutablePointer().Of<Diagnostics::Statistics>()->render.NewTime(
            Diagnostics::CalculateStopwatch(frameInformation->profilers.render));
    }
}
