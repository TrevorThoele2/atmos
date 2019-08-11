#include "Stopwatch.h"

namespace Atmos::Time
{
    Stopwatch::Stopwatch(ObjectManager& manager, Value goal) :
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

    Value Stopwatch::Start()
    {
        start = CurrentTime();
        return start;
    }

    void Stopwatch::SetStart(const Value& set)
    {
        start = set;
    }

    Value Stopwatch::GetStart() const
    {
        return start;
    }

    void Stopwatch::SetGoal(Value set)
    {
        goal = set;
    }

    Value Stopwatch::GetGoal() const
    {
        return goal;
    }

    bool Stopwatch::HasReachedGoal() const
    {
        return Elapsed() >= goal;
    }

    Value Stopwatch::Elapsed() const
    {
        return CurrentTime() - start;
    }

    Value Stopwatch::PercentageElapsed() const
    {
        return Elapsed() / goal;
    }

    ObjectTypeDescription Stopwatch::TypeDescription() const
    {
        return ObjectTraits<Stopwatch>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Time::Stopwatch>::typeName = "Stopwatch";
}

namespace Inscription
{
    Scribe<::Atmos::Time::Stopwatch, BinaryArchive>::Table::Table()
    {
        AddDataEntry(DataEntry::Auto(&ObjectT::goal, &DataT::goal));
    }

    void Scribe<::Atmos::Time::Stopwatch, BinaryArchive>::Table::ObjectScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            archive(object.CurrentTime() - object.start);
        }
        else // INPUT
        {
            ::Atmos::Time::Value timeStamp;
            archive(timeStamp);
            object.start = object.CurrentTime() - timeStamp;
        }
    }
}