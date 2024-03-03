
#pragma once

#include "StopwatchBase.h"

#include "ObjectReference.h"

#include "TimeValue.h"

#include "Fraction.h"
#include "Ratio.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    template<class Stopwatch>
    class ExtendedStopwatchAdapter
    {
    public:
        ExtendedStopwatchAdapter(ObjectManager& manager, TimeValue goal = TimeValue());
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(ExtendedStopwatchAdapter);

        bool operator==(const ExtendedStopwatchAdapter& arg) const;

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
    private:
        typedef TypedObjectReference<Stopwatch> StopwatchReference;
        StopwatchReference stopwatch;
    private:
        TimeValue elapsed;
        TimeValue average;
        TimeValue highest;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class Stopwatch>
    ExtendedStopwatchAdapter<Stopwatch>::ExtendedStopwatchAdapter(ObjectManager& manager, TimeValue goal) :
        stopwatch(manager.CreateObject<Stopwatch>(goal)),
        elapsed(TimeValue::ValueT(0)), average(TimeValue::ValueT(0)), highest(TimeValue::ValueT(0))
    {}

    template<class Stopwatch>
    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE_TEMPLATE(ExtendedStopwatchAdapter, ExtendedStopwatchAdapter<Stopwatch>) :
        INSCRIPTION_TABLE_GET_MEM(stopwatch),
        INSCRIPTION_TABLE_GET_MEM(elapsed), INSCRIPTION_TABLE_GET_MEM(average), INSCRIPTION_TABLE_GET_MEM(highest)
    {}

    template<class Stopwatch>
    bool ExtendedStopwatchAdapter<Stopwatch>::operator==(const ExtendedStopwatchAdapter& arg) const
    {
        return stopwatch == arg.stopwatch;
    }

    template<class Stopwatch>
    TimeValue ExtendedStopwatchAdapter<Stopwatch>::Start()
    {
        return stopwatch->Start();
    }

    template<class Stopwatch>
    void ExtendedStopwatchAdapter<Stopwatch>::SetGoal(TimeValue set)
    {
        stopwatch->SetGoal(set);
    }

    template<class Stopwatch>
    TimeValue ExtendedStopwatchAdapter<Stopwatch>::GetGoal() const
    {
        return stopwatch->GetGoal();
    }

    template<class Stopwatch>
    bool ExtendedStopwatchAdapter<Stopwatch>::HasReachedGoal() const
    {
        return stopwatch->HasReachedGoal();
    }

    template<class Stopwatch>
    TimeValue ExtendedStopwatchAdapter<Stopwatch>::Calculate()
    {
        elapsed = stopwatch->Elapsed();

        // Calculate highest
        if (elapsed > highest)
            highest = elapsed;

        // Calculate average
        // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
        const TimeValue::ValueT alpha(0.001, elapsed.GetRadixPoint());
        average = static_cast<TimeValue::ValueT>((alpha * elapsed.Get()) + (TimeValue::ValueT(1, 0, elapsed.GetRadixPoint()) - alpha) * average.Get());

        return elapsed;
    }

    template<class Stopwatch>
    TimeValue ExtendedStopwatchAdapter<Stopwatch>::Checkpoint()
    {
        return stopwatch->CurrentTime();
    }

    template<class Stopwatch>
    TimeValue ExtendedStopwatchAdapter<Stopwatch>::QueryElapsed() const
    {
        return elapsed;
    }

    template<class Stopwatch>
    TimeValue ExtendedStopwatchAdapter<Stopwatch>::CurrentTime() const
    {
        return stopwatch->CurrentTime();
    }

    template<class Stopwatch>
    void ExtendedStopwatchAdapter<Stopwatch>::ResetAverage()
    {
        average = TimeValue(TimeValue::ValueT(0));
    }

    template<class Stopwatch>
    TimeValue ExtendedStopwatchAdapter<Stopwatch>::GetAverage() const
    {
        return average;
    }

    template<class Stopwatch>
    void ExtendedStopwatchAdapter<Stopwatch>::ResetHighest()
    {
        highest = TimeValue(TimeValue::ValueT(0));
    }

    template<class Stopwatch>
    TimeValue ExtendedStopwatchAdapter<Stopwatch>::GetHighest() const
    {
        return highest;
    }
}

namespace Inscription
{
    template<class Stopwatch>
    class Inscripter<::Atmos::ExtendedStopwatchAdapter<Stopwatch>> : public InscripterBase<::Atmos::ExtendedStopwatchAdapter<Stopwatch>>
    {
    public:
        INSCRIPTION_INSCRIPTER_BASE_TYPEDEFS(::Atmos::ExtendedStopwatchAdapter<Stopwatch>);

        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
    };

    template<class Stopwatch>
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::ExtendedStopwatchAdapter<Stopwatch>)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD(stopwatch);
        INSCRIPTION_TABLE_ADD(elapsed);
        INSCRIPTION_TABLE_ADD(average);
        INSCRIPTION_TABLE_ADD(highest);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}