#include "DebugProfiler.h"

namespace Atmos::Debug
{
    Profiler::Profiler(StatisticPicker statisticPicker, Arca::Reliquary& reliquary) :
        statisticPicker(std::move(statisticPicker)),
        statistics(StatisticsFrom(reliquary))
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

    Statistics* Profiler::StatisticsFrom(Arca::Reliquary& reliquary)
    {
        try
        {
            return Arca::Postulate<Statistics*>(reliquary).Get();
        }
        catch (std::exception&)
        {
            return nullptr;
        }
    }
}