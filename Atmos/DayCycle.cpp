
#include "DayCycle.h"

#include "Environment.h"

namespace Atmos
{
    DayCycleHandler::DayCycleHandler() : timeFromStart()
    {}

    DayCycleHandler& DayCycleHandler::Instance()
    {
        static DayCycleHandler instance;
        return instance;
    }

    void DayCycleHandler::OnFrameEnd()
    {
        // Find the percentage of the game day
        auto &lastElapsed = Environment::GetTime().GetLastElapsed();
        Instance().timeFromStart += (lastElapsed.ConvertValue(TimeValueEpoch::MINUTES) / TimeValue::ValueT::Split(lengthGameDayInRealMinutes, 0, lastElapsed.GetRadixPoint()));
    }

    GameTimeValue DayCycleHandler::GetCurrentTime()
    {
        return Instance().timeFromStart;
    }
}