#include "StopwatchCurator.h"

#include <Arca/Reliquary.h>
#include "StopwatchCore.h"
#include "StopwatchStatistics.h"

namespace Atmos::Time
{
    StopwatchCurator::StopwatchCurator(Init init) : Curator(init)
    {}

    Value<> StopwatchCurator::Handle(const StartStopwatch& command)
    {
        auto coreData = MutablePointer<StopwatchCore>(command.id);
        coreData->start = coreData->currentTime();
        return coreData->start;
    }

    Duration<> StopwatchCurator::Handle(const CalculateStopwatch& command)
    {
        const auto coreData = MutablePointer<StopwatchCore>(command.id);

        const auto currentTime = coreData->currentTime();
        const auto elapsed = currentTime - coreData->start;

        auto statisticsData = MutablePointer<StopwatchStatistics>(command.id);
        if (statisticsData)
        {
            if (elapsed > statisticsData->highest)
                statisticsData->highest = elapsed;

            // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
            const auto alpha = Nanoseconds(1'000'000);
            statisticsData->average = 
                (alpha * elapsed.count())
                + (Nanoseconds(1'000'000'000) - alpha)
                * statisticsData->average.count();
        }

        return elapsed;
    }

    void StopwatchCurator::Handle(const ResetAverage& command)
    {
        auto statisticsData = MutablePointer<StopwatchStatistics>(command.id);
        if (statisticsData)
            statisticsData->average = Seconds();
    }

    void StopwatchCurator::Handle(const ResetHighest& command)
    {
        auto statisticsData = MutablePointer<StopwatchStatistics>(command.id);
        if (statisticsData)
            statisticsData->highest = Seconds();
    }
}