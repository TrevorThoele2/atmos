#include "DiagnosticsCurator.h"

#include "DiagnosticsStatistics.h"

namespace Atmos::Diagnostics
{
    Curator::Curator(Init init) : Arca::Curator(init)
    {}

    void Curator::Handle(const Work&)
    {
        const auto statistics = MutablePointer().Of<Statistics>();
        statistics->relicCount = Owner().RelicSize();
        statistics->shardCount = Owner().ShardSize();
    }
}
