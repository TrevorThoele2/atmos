#pragma once

#include "Stopwatch.h"

#include "ObjectReference.h"

#include "TimeValue.h"

#include "Fraction.h"
#include "Ratio.h"

#include "ObjectScribe.h"

namespace Atmos
{
    template<class Stopwatch>
    class ExtendedStopwatchAdapter;
}

namespace Inscription
{
    template<class Stopwatch>
    struct TableData<::Atmos::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive> :
        public TableDataBase<::Atmos::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive>
    {
        using ObjectT = ::Atmos::ExtendedStopwatchAdapter<Stopwatch>;

        using StopwatchReference = typename ObjectT::StopwatchReference;
        StopwatchReference stopwatch;

        ::Atmos::TimeValue elapsed;
        ::Atmos::TimeValue average;
        ::Atmos::TimeValue highest;
    };
}

namespace Atmos
{
    template<class Stopwatch>
    class ExtendedStopwatchAdapter
    {
    public:
        ExtendedStopwatchAdapter(
            ObjectManager& manager, TimeValue goal = TimeValue());
        ExtendedStopwatchAdapter(
            typename const ::Inscription::BinaryTableData<ExtendedStopwatchAdapter<Stopwatch>>& data);

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
        INSCRIPTION_TABLE_ACCESS;
    };

    template<class Stopwatch>
    ExtendedStopwatchAdapter<Stopwatch>::ExtendedStopwatchAdapter(
        ObjectManager& manager, TimeValue goal) :

        stopwatch(manager.CreateObject<Stopwatch>(goal)),
        elapsed(TimeValue::Value(0)), average(TimeValue::Value(0)), highest(TimeValue::Value(0))
    {}

    template<class Stopwatch>
    ExtendedStopwatchAdapter<Stopwatch>::ExtendedStopwatchAdapter(
        typename const ::Inscription::BinaryTableData<ExtendedStopwatchAdapter<Stopwatch>>& data) :

        stopwatch(data.stopwatch),
        stopwatch(data.elapsed, data.average, data.highest)
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
        const TimeValue::Value alpha(0.001, elapsed.GetRadixPoint());
        average = static_cast<TimeValue::Value>(
            (alpha * elapsed.Get()) +
            (TimeValue::Value(1, 0, elapsed.GetRadixPoint()) - alpha) * average.Get());

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
        average = TimeValue(TimeValue::Value(0));
    }

    template<class Stopwatch>
    TimeValue ExtendedStopwatchAdapter<Stopwatch>::GetAverage() const
    {
        return average;
    }

    template<class Stopwatch>
    void ExtendedStopwatchAdapter<Stopwatch>::ResetHighest()
    {
        highest = TimeValue(TimeValue::Value(0));
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
    class Scribe<::Atmos::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive> :
        public TableScribe<::Atmos::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive>
    {
    private:
        using BaseT = TableScribe<::Atmos::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
        using ClassNameResolver = typename BaseT::ClassNameResolver;
    public:
        using TableBase = typename BaseT::TableBase;
    public:
        class Table : public TableBase
        {
        public:
            Table()
            {
                MergeDataEntries({
                    DataEntry::Auto(&ObjectT::stopwatch, &DataT::stopwatch),
                    DataEntry::Auto(&ObjectT::elapsed, &DataT::elapsed),
                    DataEntry::Auto(&ObjectT::average, &DataT::average),
                    DataEntry::Auto(&ObjectT::highest, &DataT::highest) });
            }
        };
    };
}