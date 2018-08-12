
#include "TimerBase.h"

#include "Debug.h"
#include "Environment.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(TimerBase)
    {
        if (scribe.IsOutput())
        {
            scribe(goal);
            // Save offset
            scribe(CurrentTime() - start);
        }
        else if (scribe.IsInput())
        {
            scribe(goal);

            // Get offset
            TimeValue timeStamp;
            scribe(timeStamp);
            start = CurrentTime() - timeStamp;
            return;
        }
    }

    TimerBase::TimerBase(TimeValue goal) : goal(goal), start()
    {}

    bool TimerBase::operator==(const TimerBase &arg) const
    {
        return goal == arg.goal && start == arg.start;
    }

    bool TimerBase::operator!=(const TimerBase &arg) const
    {
        return !(*this == arg);
    }

    TimeValue TimerBase::Start()
    {
        start = CurrentTime();
        return start;
    }

    void TimerBase::SetStart(const TimeValue &set)
    {
        start = set;
    }

    TimeValue TimerBase::GetStart() const
    {
        return start;
    }

    void TimerBase::SetGoal(TimeValue set)
    {
        goal = set;
    }

    TimeValue TimerBase::GetGoal() const
    {
        return goal;
    }

    bool TimerBase::HasReachedGoal() const
    {
        return Elapsed() >= goal;
    }

    TimeValue TimerBase::Elapsed() const
    {
        return CurrentTime() - start;
    }

    TimeValue TimerBase::PercentageElapsed() const
    {
        return Elapsed() / goal;
    }
}