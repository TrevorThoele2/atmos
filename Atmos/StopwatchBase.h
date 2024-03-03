#pragma once

#include "Object.h"

#include "TimeValue.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class StopwatchBase : public Object
    {
    public:
        StopwatchBase(ObjectManager& manager, TimeValue goal = TimeValue());
        StopwatchBase(const ::Inscription::Table<StopwatchBase>& table);

        virtual ~StopwatchBase() = 0;

        bool operator==(const StopwatchBase& arg) const;
        bool operator!=(const StopwatchBase& arg) const;

        TimeValue Start();
        void SetStart(const TimeValue& set);
        TimeValue GetStart() const;

        void SetGoal(TimeValue set);
        TimeValue GetGoal() const;
        bool HasReachedGoal() const;

        TimeValue Elapsed() const;
        // Instead of 0% - 100%, it's 0 - 1 (but you probably already knew that didn't you?)
        TimeValue PercentageElapsed() const;

        virtual TimeValue CurrentTime() const = 0;

        ObjectTypeDescription TypeDescription() const override;
    private:
        TimeValue start;
        TimeValue goal;
    private:
        INSCRIPTION_ACCESS;
    };

    template<>
    struct ObjectTraits<StopwatchBase> : ObjectTraitsBase<StopwatchBase>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::StopwatchBase)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
        INSCRIPTION_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION;
    };
}