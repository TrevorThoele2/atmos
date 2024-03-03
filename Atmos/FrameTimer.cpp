
#include "FrameTimer.h"

#include "Environment.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(FrameTimer)
    {
        ::Inscription::BaseSerialize<TimerBase>(scribe, *this);
    }

    FrameTimer::FrameTimer(TimeValue goal) : TimerBase(goal)
    {}

    bool FrameTimer::operator==(const FrameTimer &arg) const
    {
        return TimerBase::operator==(arg);
    }

    bool FrameTimer::operator!=(const FrameTimer &arg) const
    {
        return !(*this == arg);
    }

    TimeValue FrameTimer::CurrentTime() const
    {
        return Environment::GetTime().GetTotalElapsed();
    }
}