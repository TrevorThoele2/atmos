#pragma once

#include "GameTimer.h"

namespace Atmos
{
    class DayCycleHandler
    {
    public:
        typedef GameTimer::CountT CountT;
        static constexpr CountT hoursPerDay = GameTimer::hoursPerDay;
        static constexpr CountT minutesPerHour = GameTimer::minutesPerHour;
        static constexpr CountT secondsPerMinute = GameTimer::secondsPerMinute;
        static constexpr CountT millisecondsPerSecond = GameTimer::millisecondsPerSecond;
    private:
        static constexpr CountT lengthGameDayInRealMinutes = 30;
    private:
        // Measures from the start of the game
        GameTimeValue timeFromStart;

        static constexpr double dayLength = 24.0;
        static constexpr double sunRiseStart = 5.0;
        static constexpr double sunRiseStop = 6.0;
        static constexpr double sunSetStart = 20.0;
        static constexpr double sunSetStop = 21.0;

        DayCycleHandler();
        DayCycleHandler(const DayCycleHandler &arg) = delete;
        DayCycleHandler& operator=(const DayCycleHandler &arg) = delete;
    public:
        static DayCycleHandler& Instance();
        static void OnFrameEnd();
        static GameTimeValue GetCurrentTime();
    };
}