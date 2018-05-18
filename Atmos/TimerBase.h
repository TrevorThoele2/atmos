
#pragma once

#include "TimeValue.h"

#include "Serialization.h"

namespace Atmos
{
    // Goal is in seconds
    class TimerBase
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        TimeValue start;
        TimeValue goal;
    public:
        TimerBase(TimeValue goal = TimeValue());
        virtual ~TimerBase() = 0 {}
        bool operator==(const TimerBase &arg) const;
        bool operator!=(const TimerBase &arg) const;

        TimeValue Start();
        void SetStart(const TimeValue &set);
        TimeValue GetStart() const;

        void SetGoal(TimeValue set);
        TimeValue GetGoal() const;
        bool HasReachedGoal() const;

        TimeValue Elapsed() const;
        // Instead of 0% - 100%, it's 0 - 1 (but you probably already knew that didn't you?)
        TimeValue PercentageElapsed() const;

        virtual TimeValue CurrentTime() const = 0;
    };
}