#include "StopwatchCore.h"

namespace Atmos::Time
{
    StopwatchCore::StopwatchCore(std::function<Value<>()>&& currentTime) : currentTime(std::move(currentTime))
    {}
}