#include "CreateStopwatch.h"

namespace Atmos::Time
{
    Stopwatch CreateRealStopwatch()
    {
        return Stopwatch(
            []()
            {
                return std::chrono::high_resolution_clock::now();
            });
    }

    Stopwatch CreateFrameStopwatch(const Frame::Information& information)
    {
        return Stopwatch(
            [&information]()
            {
                return Time::Point<>() + information.totalElapsed;
            });
    }
}