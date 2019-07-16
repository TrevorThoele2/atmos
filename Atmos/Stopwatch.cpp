#include "Stopwatch.h"

namespace Atmos
{
    Stopwatch::Stopwatch(ObjectManager& manager, TimeValue goal) :
        Object(manager), goal(goal), start()
    {}

    Stopwatch::Stopwatch(const ::Inscription::BinaryTableData<Stopwatch>& data) :
        Object(std::get<0>(data.bases)), goal(data.goal), start()
    {}

    Stopwatch::~Stopwatch()
    {}

    bool Stopwatch::operator==(const Stopwatch& arg) const
    {
        return goal == arg.goal && start == arg.start;
    }

    bool Stopwatch::operator!=(const Stopwatch& arg) const
    {
        return !(*this == arg);
    }

    TimeValue Stopwatch::Start()
    {
        start = CurrentTime();
        return start;
    }

    void Stopwatch::SetStart(const TimeValue& set)
    {
        start = set;
    }

    TimeValue Stopwatch::GetStart() const
    {
        return start;
    }

    void Stopwatch::SetGoal(TimeValue set)
    {
        goal = set;
    }

    TimeValue Stopwatch::GetGoal() const
    {
        return goal;
    }

    bool Stopwatch::HasReachedGoal() const
    {
        return Elapsed() >= goal;
    }

    TimeValue Stopwatch::Elapsed() const
    {
        return CurrentTime() - start;
    }

    TimeValue Stopwatch::PercentageElapsed() const
    {
        return Elapsed() / goal;
    }

    ObjectTypeDescription Stopwatch::TypeDescription() const
    {
        return ObjectTraits<Stopwatch>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<Stopwatch>::typeName = "Stopwatch";
}

namespace Inscription
{
    Scribe<::Atmos::Stopwatch, BinaryArchive>::Table::Table()
    {
        AddDataEntry(DataEntry::Auto(&ObjectT::goal, &DataT::goal));
    }

    void Scribe<::Atmos::Stopwatch, BinaryArchive>::Table::ObjectScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            archive(object.CurrentTime() - object.start);
        }
        else // INPUT
        {
            ::Atmos::TimeValue timeStamp;
            archive(timeStamp);
            object.start = object.CurrentTime() - timeStamp;
        }
    }
}