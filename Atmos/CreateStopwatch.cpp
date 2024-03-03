#include "CreateStopwatch.h"

namespace Atmos::Time
{
    Stopwatch CreateRealStopwatch()
    {
        return Stopwatch([]() { return std::chrono::high_resolution_clock::now(); });
    }

    Stopwatch CreateFrameStopwatch(Frame::Information& information)
    {
        return Stopwatch([&information]() { return Time::Value<>() + information.totalElapsed; });
    }
}