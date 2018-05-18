
#pragma once

#include "TimeValue.h"
#include "Fraction.h"
#include "Ratio.h"

namespace Atmos
{
    // Used for additional diagnostics than the regular timers
    template<class TimerT>
    class ExtendedTimer
    {
    private:
        TimerT wrapped;

        TimeValue elapsed;
        TimeValue average;
        TimeValue highest;
    public:
        ExtendedTimer(TimeValue goal = TimeValue());
        bool operator==(const ExtendedTimer &arg) const;
        TimeValue Start();
        void SetGoal(TimeValue set);
        TimeValue GetGoal() const;
        bool HasReachedGoal() const;
        TimeValue Calculate();
        TimeValue Checkpoint();
        // Doesn't calculate anything
        TimeValue QueryElapsed() const;
        TimeValue CurrentTime() const;

        void ResetAverage();
        TimeValue GetAverage() const;
        void ResetHighest();
        TimeValue GetHighest() const;
    };

    template<class TimerT>
    ExtendedTimer<TimerT>::ExtendedTimer(TimeValue goal) : wrapped(goal), elapsed(TimeValue::ValueT(0)), average(TimeValue::ValueT(0)), highest(TimeValue::ValueT(0))
    {}

    template<class TimerT>
    bool ExtendedTimer<TimerT>::operator==(const ExtendedTimer &arg) const
    {
        return wrapped == arg.wrapped;
    }

    template<class TimerT>
    TimeValue ExtendedTimer<TimerT>::Start()
    {
        return wrapped.Start();
    }

    template<class TimerT>
    void ExtendedTimer<TimerT>::SetGoal(TimeValue set)
    {
        wrapped.SetGoal(set);
    }

    template<class TimerT>
    TimeValue ExtendedTimer<TimerT>::GetGoal() const
    {
        return wrapped.GetGoal();
    }

    template<class TimerT>
    bool ExtendedTimer<TimerT>::HasReachedGoal() const
    {
        return wrapped.HasReachedGoal();
    }

    template<class TimerT>
    TimeValue ExtendedTimer<TimerT>::Calculate()
    {
        elapsed = wrapped.Elapsed();

        // Calculate highest
        if (elapsed > highest)
            highest = elapsed;

        // Calculate average
        // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
        const TimeValue::ValueT alpha(0.001, elapsed.GetRadixPoint());
        average = static_cast<TimeValue::ValueT>((alpha * elapsed.Get()) + (TimeValue::ValueT(1, 0, elapsed.GetRadixPoint()) - alpha) * average.Get());

        return elapsed;
    }

    template<class TimerT>
    TimeValue ExtendedTimer<TimerT>::Checkpoint()
    {
        return wrapped.CurrentTime();
    }

    template<class TimerT>
    TimeValue ExtendedTimer<TimerT>::QueryElapsed() const
    {
        return elapsed;
    }

    template<class TimerT>
    TimeValue ExtendedTimer<TimerT>::CurrentTime() const
    {
        return wrapped.CurrentTime();
    }

    template<class TimerT>
    void ExtendedTimer<TimerT>::ResetAverage()
    {
        average = TimeValue(TimeValue::ValueT(0));
    }

    template<class TimerT>
    TimeValue ExtendedTimer<TimerT>::GetAverage() const
    {
        return average;
    }

    template<class TimerT>
    void ExtendedTimer<TimerT>::ResetHighest()
    {
        highest = TimeValue(TimeValue::ValueT(0));
    }

    template<class TimerT>
    TimeValue ExtendedTimer<TimerT>::GetHighest() const
    {
        return highest;
    }
}