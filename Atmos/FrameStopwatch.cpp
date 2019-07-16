
#include "FrameStopwatch.h"

#include "ObjectManager.h"
#include "TimeSystem.h"

namespace Atmos
{
    FrameStopwatch::FrameStopwatch(ObjectManager& manager, TimeValue goal) : Stopwatch(manager, goal)
    {
        timeSystem = Manager()->FindSystem<TimeSystem>();
    }

    FrameStopwatch::FrameStopwatch(const ::Inscription::BinaryTableData<FrameStopwatch>& data) :
        Stopwatch(std::get<0>(data.bases))
    {
        timeSystem = Manager()->FindSystem<TimeSystem>();
    }

    bool FrameStopwatch::operator==(const FrameStopwatch& arg) const
    {
        return Stopwatch::operator==(arg);
    }

    bool FrameStopwatch::operator!=(const FrameStopwatch& arg) const
    {
        return !(*this == arg);
    }

    TimeValue FrameStopwatch::CurrentTime() const
    {
        return timeSystem->GetTotalElapsed();
    }

    ObjectTypeDescription FrameStopwatch::TypeDescription() const
    {
        return ObjectTraits<FrameStopwatch>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<FrameStopwatch>::typeName = "FrameStopwatch";
}