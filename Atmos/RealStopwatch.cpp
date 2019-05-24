
#include "RealStopwatch.h"

#include "ObjectManager.h"
#include "TimeSystem.h"

namespace Atmos
{
    RealStopwatch::RealStopwatch(ObjectManager& manager, TimeValue goal) : StopwatchBase(manager, goal)
    {
        timeSystem = manager.FindSystem<TimeSystem>();
    }

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(RealStopwatch) : INSCRIPTION_TABLE_GET_BASE(StopwatchBase)
    {
        timeSystem = Manager()->FindSystem<TimeSystem>();
    }

    bool RealStopwatch::operator==(const RealStopwatch& arg) const
    {
        return StopwatchBase::operator==(arg);
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

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::RealStopwatch)
    {

    }
}