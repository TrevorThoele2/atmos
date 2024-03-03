#include "DebugStatistics.h"

#include "CreateStopwatch.h"

namespace Atmos::Debug
{
    Statistics::Profilers::Profilers() :
        input(CreateProfiler()),
        logic(CreateProfiler()),
        render(CreateProfiler()),
        frameTime(CreateProfiler()),
        idle(CreateProfiler()),
        misc1(CreateProfiler()),
        misc2(CreateProfiler()),
        misc3(CreateProfiler())
    {}

    auto Statistics::Profilers::CreateProfiler() -> Time::Stopwatch
    {
        return Time::CreateRealStopwatch();
    }
}