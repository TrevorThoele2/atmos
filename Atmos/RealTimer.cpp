
#include "RealTimer.h"

#include "Environment.h"
#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(RealTimer)
    {
        ::inscription::BaseSerialize<TimerBase>(scribe, *this);
    }

    RealTimer::RealTimer(TimeValue goal) : TimerBase(goal)
    {}

    bool RealTimer::operator==(const RealTimer &arg) const
    {
        return TimerBase::operator==(arg);
    }

    TimeValue RealTimer::CurrentTime() const
    {
        return Environment::GetTime().CurrentTime();
    }
}