
#include "StopwatchBase.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>

namespace Atmos
{
    StopwatchBase::StopwatchBase(ObjectManager& manager, TimeValue goal) :
        Object(manager), goal(goal), start()
    {}

    StopwatchBase::StopwatchBase(const ::Inscription::Table<StopwatchBase>& table) :
        INSCRIPTION_TABLE_GET_BASE(Object), INSCRIPTION_TABLE_GET_MEM(goal)
    {}

    StopwatchBase::~StopwatchBase()
    {}

    bool StopwatchBase::operator==(const StopwatchBase& arg) const
    {
        return goal == arg.goal && start == arg.start;
    }

    bool StopwatchBase::operator!=(const StopwatchBase& arg) const
    {
        return !(*this == arg);
    }

    TimeValue StopwatchBase::Start()
    {
        start = CurrentTime();
        return start;
    }

    void StopwatchBase::SetStart(const TimeValue& set)
    {
        start = set;
    }

    TimeValue StopwatchBase::GetStart() const
    {
        return start;
    }

    void StopwatchBase::SetGoal(TimeValue set)
    {
        goal = set;
    }

    TimeValue StopwatchBase::GetGoal() const
    {
        return goal;
    }

    bool StopwatchBase::HasReachedGoal() const
    {
        return Elapsed() >= goal;
    }

    TimeValue StopwatchBase::Elapsed() const
    {
        return CurrentTime() - start;
    }

    TimeValue StopwatchBase::PercentageElapsed() const
    {
        return Elapsed() / goal;
    }

    ObjectTypeDescription StopwatchBase::TypeDescription() const
    {
        return ObjectTraits<StopwatchBase>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<StopwatchBase>::typeName = "StopwatchBase";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::StopwatchBase)
    {
        INSCRIPTION_TABLE_ADD(goal);
    }

    INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::StopwatchBase)
    {
        INSCRIPTION_INSCRIPTER_CALL_BASE_SERIALIZE_FUNCTION;
        if (scribe.IsOutput())
        {
            scribe(obj.CurrentTime() - obj.start);
        }
        else // INPUT
        {
            ::Atmos::TimeValue timeStamp;
            scribe(timeStamp);
            obj.start = obj.CurrentTime() - timeStamp;
        }
    }
}