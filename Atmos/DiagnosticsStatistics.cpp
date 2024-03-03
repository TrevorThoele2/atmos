#include "DiagnosticsStatistics.h"

namespace Atmos::Diagnostics
{
    bool Statistics::operator==(const Statistics& arg) const
    {
        return relicCount == arg.relicCount
            && shardCount == arg.shardCount
            && nextRelicID == arg.nextRelicID
            && renderTime == arg.renderTime
            && idleTime == arg.idleTime;
    }

    bool Statistics::operator!=(const Statistics& arg) const
    {
        return !(*this == arg);
    }
}