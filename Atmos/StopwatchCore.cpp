#include "StopwatchCore.h"

namespace Atmos::Time
{
    StopwatchCore::StopwatchCore(std::function<Value<>()>&& currentTime) : currentTime(std::move(currentTime))
    {}

    bool StopwatchCore::IsStarted() const
    {
        return start != Value<>();
    }
}