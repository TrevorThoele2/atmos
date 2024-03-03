#include "DebugProfiler.h"

namespace Atmos::Debug
{
    Profiler::Profiler(StatisticPicker statisticPicker, Arca::MutablePointer mutablePointer) :
        statisticPicker(std::move(statisticPicker)),
        statistics(StatisticsFrom(mutablePointer))
    {}

    void Profiler::Start()
    {
        if (!statistics)
            return;

        statisticPicker(*statistics).Restart();
    }

    void Profiler::Calculate()
    {
        if (!statistics)
            return;

        statisticPicker(*statistics).Calculate();
    }

    Statistics* Profiler::StatisticsFrom(Arca::MutablePointer mutablePointer)
    {
        try
        {
            const Arca::Index<Statistics> index(mutablePointer.Reliquary());
            return mutablePointer.Of(index);
        }
        catch(Arca::NotRegistered&)
        {
            return nullptr;
        }
    }
}