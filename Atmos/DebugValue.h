#pragma once

#include <functional>

#include "DebugStatistics.h"

namespace Atmos::Debug
{
    class Value
    {
    public:
        using StatisticSetter = std::function<void(Statistics&)>;
    public:
        Value(StatisticSetter statisticSetter, Arca::Reliquary& reliquary);

        void Set();
    private:
        StatisticSetter statisticSetter;

        Statistics* statistics;

        static Statistics* StatisticsFrom(Arca::Reliquary& reliquary);
    };
}