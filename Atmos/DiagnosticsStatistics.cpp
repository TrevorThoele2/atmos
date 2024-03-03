#include "DiagnosticsStatistics.h"

namespace Atmos::Diagnostics
{
    double CalculateStopwatch(Time::Stopwatch& stopwatch)
    {
        const auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stopwatch.Calculate());
        return duration.count();
    }

    bool Statistics::Profile::operator==(const Profile& arg) const
    {
        return time == arg.time && average == arg.average && highest == arg.highest;
    }

    bool Statistics::Profile::operator!=(const Profile& arg) const
    {
        return !(*this == arg);
    }

    void Statistics::Profile::NewTime(Time::Stopwatch& stopwatch)
    {
        constexpr double maxAverageCount = 1000;
        const auto newTime = CalculateStopwatch(stopwatch);
        *this =
        {
            newTime,
            (average * (maxAverageCount - 1) + newTime) / maxAverageCount,
            std::max(highest, newTime)
        };
    }

    bool Statistics::operator==(const Statistics& arg) const
    {
        return relicCount == arg.relicCount
            && shardCount == arg.shardCount
            && nextRelicID == arg.nextRelicID
            && script == arg.script
            && render == arg.render
            && frame == arg.frame
            && idle == arg.idle
            && misc == arg.misc;
    }

    bool Statistics::operator!=(const Statistics& arg) const
    {
        return !(*this == arg);
    }
}