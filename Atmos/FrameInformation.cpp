#include "FrameInformation.h"

#include "CreateStopwatch.h"

namespace Atmos::Frame
{
    Information::Profilers::Profilers() :
        render(Time::CreateRealStopwatch()), frame(Time::CreateRealStopwatch()), idle(Time::CreateRealStopwatch())
    {}
}