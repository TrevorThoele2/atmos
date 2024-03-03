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
        Value(StatisticSetter statisticSetter, Arca::MutablePointer mutablePointer);

        void Set();
    private:
        StatisticSetter statisticSetter;

        Statistics* statistics;

        static Statistics* StatisticsFrom(Arca::MutablePointer mutablePointer);
    };
}