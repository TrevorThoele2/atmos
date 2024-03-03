#pragma once

#include "Stopwatch.h"

#include "ObjectReference.h"

#include "TimeValue.h"

#include "Fraction.h"
#include "Ratio.h"

#include "ObjectScribe.h"

namespace Atmos::Time
{
    template<class Stopwatch>
    class ExtendedStopwatchAdapter;
}

namespace Inscription
{
    template<class Stopwatch>
    struct TableData<::Atmos::Time::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive> :
        public TableDataBase<::Atmos::Time::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive>
    {
        using ObjectT = ::Atmos::Time::ExtendedStopwatchAdapter<Stopwatch>;

        using StopwatchReference = typename ObjectT::StopwatchReference;
        StopwatchReference stopwatch;

        ::Atmos::Time::Value elapsed;
        ::Atmos::Time::Value average;
        ::Atmos::Time::Value highest;
    };
}

namespace Atmos::Time
{
    template<class Stopwatch>
    class ExtendedStopwatchAdapter
    {
    public:
        ExtendedStopwatchAdapter(
            ObjectManager& manager, Value goal = Value());
        ExtendedStopwatchAdapter(
            typename const ::Inscription::BinaryTableData<ExtendedStopwatchAdapter<Stopwatch>>& data);

        bool operator==(const ExtendedStopwatchAdapter& arg) const;

        Value Start();
        void SetGoal(Value set);
        Value GetGoal() const;
        bool HasReachedGoal() const;
        Value Calculate();
        Value Checkpoint();
        // Doesn't calculate anything
        Value QueryElapsed() const;
        Value CurrentTime() const;

        void ResetAverage();
        Value GetAverage() const;
        void ResetHighest();
        Value GetHighest() const;
    private:
        typedef TypedObjectReference<Stopwatch> StopwatchReference;
        StopwatchReference stopwatch;
    private:
        Value elapsed;
        Value average;
        Value highest;
    private:
        INSCRIPTION_TABLE_ACCESS;
    };

    template<class Stopwatch>
    ExtendedStopwatchAdapter<Stopwatch>::ExtendedStopwatchAdapter(
        ObjectManager& manager, Value goal) :

        stopwatch(manager.CreateObject<Stopwatch>(goal)),
        elapsed(Value::Number(0)), average(Value::Number(0)), highest(Value::Number(0))
    {}

    template<class Stopwatch>
    ExtendedStopwatchAdapter<Stopwatch>::ExtendedStopwatchAdapter(
        typename const ::Inscription::BinaryTableData<ExtendedStopwatchAdapter<Stopwatch>>& data) :

        stopwatch(data.stopwatch),
        elapsed(data.elapsed), average(data.average), highest(data.highest)
    {}

    template<class Stopwatch>
    bool ExtendedStopwatchAdapter<Stopwatch>::operator==(const ExtendedStopwatchAdapter& arg) const
    {
        return stopwatch == arg.stopwatch;
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::Start()
    {
        return stopwatch->Start();
    }

    template<class Stopwatch>
    void ExtendedStopwatchAdapter<Stopwatch>::SetGoal(Value set)
    {
        stopwatch->SetGoal(set);
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::GetGoal() const
    {
        return stopwatch->GetGoal();
    }

    template<class Stopwatch>
    bool ExtendedStopwatchAdapter<Stopwatch>::HasReachedGoal() const
    {
        return stopwatch->HasReachedGoal();
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::Calculate()
    {
        elapsed = stopwatch->Elapsed();

        // Calculate highest
        if (elapsed > highest)
            highest = elapsed;

        // Calculate average
        // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
        const Value::Number alpha(0.001, elapsed.GetRadixPoint());
        average = static_cast<Value::Number>(
            (alpha * elapsed.Get()) +
            (Value::Number(1, 0, elapsed.GetRadixPoint()) - alpha) * average.Get());

        return elapsed;
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::Checkpoint()
    {
        return stopwatch->CurrentTime();
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::QueryElapsed() const
    {
        return elapsed;
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::CurrentTime() const
    {
        return stopwatch->CurrentTime();
    }

    template<class Stopwatch>
    void ExtendedStopwatchAdapter<Stopwatch>::ResetAverage()
    {
        average = Value(Value::Number(0));
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::GetAverage() const
    {
        return average;
    }

    template<class Stopwatch>
    void ExtendedStopwatchAdapter<Stopwatch>::ResetHighest()
    {
        highest = Value(Value::Number(0));
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::GetHighest() const
    {
        return highest;
    }
}

namespace Inscription
{
    template<class Stopwatch>
    class Scribe<::Atmos::Time::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive> :
        public TableScribe<::Atmos::Time::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive>
    {
    private:
        using BaseT = TableScribe<::Atmos::Time::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
        using ClassNameResolver = typename BaseT::ClassNameResolver;

        using BaseT::Scriven;
        using BaseT::Construct;
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
        protected:
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
            {
                DoBasicConstruction(storage, archive);
            }

            using TableBase::DoBasicConstruction;
        };
    };
}