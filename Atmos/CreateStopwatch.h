#pragma once

#include "Stopwatch.h"
#include "FrameInformation.h"

namespace Atmos::Time
{
    Stopwatch CreateRealStopwatch();
    Stopwatch CreateFrameStopwatch(Frame::Information& information);
}