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
        
        const auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(frameInformation->profilers.render.Calculate());
        MutablePointer().Of<Diagnostics::Statistics>()->renderTime = duration.count();
    }
}
