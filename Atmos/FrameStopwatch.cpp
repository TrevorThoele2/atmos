
#include "FrameStopwatch.h"

#include "ObjectManager.h"
#include "TimeSystem.h"

namespace Atmos
{
    FrameStopwatch::FrameStopwatch(ObjectManager& manager, TimeValue goal) : StopwatchBase(manager, goal)
    {
        timeSystem = Manager()->FindSystem<TimeSystem>();
    }

    FrameStopwatch::FrameStopwatch(const ::Inscription::Table<FrameStopwatch>& table) : INSCRIPTION_TABLE_GET_BASE(StopwatchBase)
    {
        timeSystem = Manager()->FindSystem<TimeSystem>();
    }

    bool FrameStopwatch::operator==(const FrameStopwatch& arg) const
    {
        return StopwatchBase::operator==(arg);
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

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::FrameStopwatch)
    {

    }
}