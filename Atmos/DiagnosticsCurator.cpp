#include "DiagnosticsCurator.h"

#include "DiagnosticsStatistics.h"
#include "FrameInformation.h"

namespace Atmos::Diagnostics
{
    Curator::Curator(Init init) : Arca::Curator(init)
    {}

    void Curator::Handle(const Work&)
    {
        const auto frameInformation = Owner().Find<Frame::Information>();

        const auto statistics = MutablePointer().Of<Statistics>();
        statistics->relicCount = Owner().RelicSize();
        statistics->shardCount = Owner().ShardSize();
        statistics->nextRelicID = Owner().NextRelicID();
        statistics->framesPerSecond = frameInformation->framesPerSecond;
    }
}
