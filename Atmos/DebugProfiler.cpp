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

        statisticPicker(*statistics)->Start();
    }

    void Profiler::Calculate()
    {
        if (!statistics)
            return;

        statisticPicker(*statistics)->Elapsed();
    }

    Statistics* Profiler::StatisticsFrom(Arca::MutablePointer mutablePointer)
    {
        try
        {
            const Arca::Postulate<Statistics*> postulate(mutablePointer.Reliquary());
            return mutablePointer.Of(*postulate);
        }
        catch(Arca::NotRegistered&)
        {
            return nullptr;
        }
    }
}