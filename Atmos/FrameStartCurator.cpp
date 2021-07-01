#include "FrameStartCurator.h"

#include "FrameInformation.h"

#include "DiagnosticsStatistics.h"

namespace Atmos::Frame
{
    StartCurator::StartCurator(Init init) : Curator(init)
    {}

    void StartCurator::Handle(const Work&)
    {
        auto mutableInformation = MutablePointer().Of<Information>();
        mutableInformation->startTime = mutableInformation->profilers.frame.Restart();

        const auto idleDuration = std::chrono::duration_cast<std::chrono::duration<double>>(mutableInformation->profilers.idle.Calculate());
        MutablePointer().Of<Diagnostics::Statistics>()->idleTime = idleDuration.count();
    }
}
