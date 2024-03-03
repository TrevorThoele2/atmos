#pragma once

#include "Stopwatch.h"

#include "TimeValue.h"

#include "Ratio.h"

namespace Atmos::Time
{
    template<class Stopwatch>
    class ExtendedStopwatchAdapter
    {
    public:
        using SourceT = Stopwatch;
    public:
        ExtendedStopwatchAdapter();
        explicit ExtendedStopwatchAdapter(Arca::Ptr<SourceT> source);
        explicit ExtendedStopwatchAdapter(const ::Inscription::BinaryTableData<ExtendedStopwatchAdapter>& data);

        bool operator==(const ExtendedStopwatchAdapter& arg) const;

        void Source(Arca::Ptr<SourceT> source);
        Arca::Ptr<SourceT> Source() const;

        Value Start();
        Value Calculate();
        Value Checkpoint();
        // Doesn't calculate anything
        [[nodiscard]] Value QueryElapsed() const;
        [[nodiscard]] Value CurrentTime() const;

        void ResetAverage();
        [[nodiscard]] Value GetAverage() const;
        void ResetHighest();
        [[nodiscard]] Value GetHighest() const;
    private:
        Arca::Ptr<SourceT> source;

        Value elapsed = Value(Value::Number(0));
        Value average = Value(Value::Number(0));
        Value highest = Value(Value::Number(0));
    private:
        INSCRIPTION_TABLE_ACCESS;
    };

    template<class Stopwatch>
    ExtendedStopwatchAdapter<Stopwatch>::ExtendedStopwatchAdapter() = default;

    template<class Stopwatch>
    ExtendedStopwatchAdapter<Stopwatch>::ExtendedStopwatchAdapter(Arca::Ptr<SourceT> source) :
        source(source)
    {}

    template<class Stopwatch>
    ExtendedStopwatchAdapter<Stopwatch>::ExtendedStopwatchAdapter
    (
        const ::Inscription::BinaryTableData<ExtendedStopwatchAdapter<Stopwatch>>& data
    ) :
        source(data.source),
        elapsed(data.elapsed), average(data.average), highest(data.highest)
    {}

    template<class Stopwatch>
    bool ExtendedStopwatchAdapter<Stopwatch>::operator==(const ExtendedStopwatchAdapter& arg) const
    {
        return source == arg.source;
    }

    template<class Stopwatch>
    void ExtendedStopwatchAdapter<Stopwatch>::Source(Arca::Ptr<SourceT> source)
    {
        this->source = source;
    }

    template<class Stopwatch>
    auto ExtendedStopwatchAdapter<Stopwatch>::Source() const -> Arca::Ptr<SourceT>
    {
        return source;
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::Start()
    {
        return source->Start();
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::Calculate()
    {
        elapsed = source->Elapsed();

        // Calculate highest
        if (elapsed > highest)
            highest = elapsed;

        // Calculate average
        // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
        const Value::Number alpha(0.001, elapsed.GetRadixPoint());
        average = Value(static_cast<Value::Number>(
            (alpha * elapsed.Get()) +
            (Value::Number(1, 0, elapsed.GetRadixPoint()) - alpha) * average.Get()));

        return elapsed;
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::Checkpoint()
    {
        return source->CurrentTime();
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::QueryElapsed() const
    {
        return elapsed;
    }

    template<class Stopwatch>
    Value ExtendedStopwatchAdapter<Stopwatch>::CurrentTime() const
    {
        return source->CurrentTime();
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
    struct TableData<::Atmos::Time::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive> :
        TableDataBase<::Atmos::Time::ExtendedStopwatchAdapter<Stopwatch>, BinaryArchive>
    {
        using ObjectT = ::Atmos::Time::ExtendedStopwatchAdapter<Stopwatch>;

        using SourceT = typename ObjectT::SourceT;
        Arca::Ptr<SourceT> source;

        ::Atmos::Time::Value elapsed;
        ::Atmos::Time::Value average;
        ::Atmos::Time::Value highest;
    };

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
        using DataT = typename BaseT::DataT;
        using TableBase = typename BaseT::TableBase;
    public:
        class Table : public TableBase
        {
        public:
            Table()
            {
                MergeDataEntries
                ({
                    DataEntry::Auto(&ObjectT::source, &DataT::source),
                    DataEntry::Auto(&ObjectT::elapsed, &DataT::elapsed),
                    DataEntry::Auto(&ObjectT::average, &DataT::average),
                    DataEntry::Auto(&ObjectT::highest, &DataT::highest) }
                );
            }
        protected:
            using DataEntry = typename BaseT::DataEntry;
        };
    };
}