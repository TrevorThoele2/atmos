#include "FrameInformation.h"

#include "CreateStopwatch.h"

namespace Atmos::Frame
{
    Information::Information() : stopwatch(Time::CreateRealStopwatch())
    {}
}