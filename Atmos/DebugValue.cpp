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
            const Arca::Postulate<Statistics*> postulate(mutablePointer.Reliquary());
            return mutablePointer.Of(*postulate);
        }
        catch (Arca::NotRegistered&)
        {
            return nullptr;
        }
    }
}