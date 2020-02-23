#pragma once

#include <functional>

#include "DebugStatistics.h"

namespace Atmos::Debug
{
    class Profiler
    {
    public:
        using Statistic = Arca::Index<Time::RealStopwatch>;
        using StatisticPicker = std::function<Statistic&(Statistics&)>;
    public:
        Profiler(StatisticPicker statisticPicker, Arca::Reliquary& reliquary);

        void Start();
        void Calculate();
    private:
        StatisticPicker statisticPicker;

        Statistics* statistics;

        static Statistics* StatisticsFrom(Arca::Reliquary& reliquary);
    };
}