#include "RealStopwatch.h"

#include "ObjectManager.h"
#include "TimeSystem.h"

namespace Atmos::Time
{
    RealStopwatch::RealStopwatch(ObjectManager& manager, Value goal) :
        Stopwatch(manager, goal)
    {
        timeSystem = manager.FindSystem<TimeSystem>();
    }

    RealStopwatch::RealStopwatch(const ::Inscription::BinaryTableData<RealStopwatch>& data) :
        Stopwatch(std::get<0>(data.bases))
    {}

    bool RealStopwatch::operator==(const RealStopwatch& arg) const
    {
        return Stopwatch::operator==(arg);
    }

    Value RealStopwatch::CurrentTime() const
    {
        return timeSystem->CurrentTime();
    }

    ObjectTypeDescription RealStopwatch::TypeDescription() const
    {
        return ObjectTraits<RealStopwatch>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Time::RealStopwatch>::typeName = "RealStopwatch";
}