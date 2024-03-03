#pragma once

#include <functional>

#include "DebugStatistics.h"
#include "Stopwatch.h"

namespace Atmos::Debug
{
    class Profiler
    {
    public:
        using Statistic = Time::Stopwatch;
        using StatisticPicker = std::function<Statistic&(Statistics&)>;
    public:
        Profiler(StatisticPicker statisticPicker, Arca::MutablePointer mutablePointer);

        void Start();
        void Calculate();
    private:
        StatisticPicker statisticPicker;

        Statistics* statistics;

        static Statistics* StatisticsFrom(Arca::MutablePointer mutablePointer);
    };
}