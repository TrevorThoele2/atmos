#include "DebugValue.h"

namespace Atmos::Debug
{
    Value::Value(StatisticSetter statisticSetter, Arca::MutablePointer mutablePointer) :
        statisticSetter(std::move(statisticSetter)),
        statistics(StatisticsFrom(mutablePointer))
    {}

    void Value::Set()
    {
        if (!statistics)
            return;

        statisticSetter(*statistics);
    }

    Statistics* Value::StatisticsFrom(Arca::MutablePointer mutablePointer)
    {
        try
        {
            const Arca::Index<Statistics> index(mutablePointer.Reliquary());
            return mutablePointer.Of(index);
        }
        catch (Arca::NotRegistered&)
        {
            return nullptr;
        }
    }
}