#include "StopwatchCurator.h"

#include "StopwatchCore.h"
#include "StopwatchStatistics.h"

namespace Atmos::Time
{
    Value StopwatchCurator::Handle(const StartStopwatch& command)
    {
        auto coreData = Data<StopwatchCore>(command.id);
        coreData->start = coreData->currentTime();
        return coreData->start;
    }

    Value StopwatchCurator::Handle(const CalculateStopwatch& command)
    {
        const auto coreData = Data<StopwatchCore>(command.id);

        const auto elapsed = coreData->currentTime() - coreData->start;

        auto statisticsData = Data<StopwatchStatistics>(command.id);
        if (statisticsData)
        {
            if (elapsed > statisticsData->highest)
                statisticsData->highest = elapsed;

            // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
            const Value::Number alpha(0.001, elapsed.GetRadixPoint());
            statisticsData->average = Value(static_cast<Value::Number>(
                (alpha * elapsed.Get()) +
                (Value::Number(1, 0, elapsed.GetRadixPoint()) - alpha) * statisticsData->average.Get()));
        }

        return elapsed;
    }

    void StopwatchCurator::Handle(const ResetAverage& command)
    {
        auto statisticsData = Data<StopwatchStatistics>(command.id);
        if (statisticsData)
            statisticsData->average = Value(Value::Number(0));
    }

    void StopwatchCurator::Handle(const ResetHighest& command)
    {
        auto statisticsData = Data<StopwatchStatistics>(command.id);
        if (statisticsData)
            statisticsData->highest = Value(Value::Number(0));
    }
}