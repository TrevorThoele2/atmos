#include "FrameInformation.h"

#include "CreateStopwatch.h"

namespace Atmos::Frame
{
    Information::Information(Init init) :
        ClosedTypedRelic(init),
        stopwatch(Time::CreateRealStopwatch())
    {}
}