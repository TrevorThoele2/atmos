#include "DebugValue.h"

namespace Atmos::Debug
{
    Value::Value(StatisticSetter statisticSetter, Arca::Reliquary& reliquary) :
        statisticSetter(std::move(statisticSetter)),
        statistics(StatisticsFrom(reliquary))
    {}

    void Value::Set()
    {
        if (!statistics)
            return;

        statisticSetter(*statistics);
    }

    Statistics* Value::StatisticsFrom(Arca::Reliquary& reliquary)
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