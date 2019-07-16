
#include "RealStopwatch.h"

#include "ObjectManager.h"
#include "TimeSystem.h"

namespace Atmos
{
    RealStopwatch::RealStopwatch(ObjectManager& manager, TimeValue goal) :
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

    TimeValue RealStopwatch::CurrentTime() const
    {
        return timeSystem->CurrentTime();
    }

    ObjectTypeDescription RealStopwatch::TypeDescription() const
    {
        return ObjectTraits<RealStopwatch>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<RealStopwatch>::typeName = "RealStopwatch";
}