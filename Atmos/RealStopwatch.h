#pragma once

#include "StopwatchBase.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class TimeSystem;

    class RealStopwatch : public StopwatchBase
    {
    public:
        RealStopwatch(ObjectManager& manager, TimeValue goal = TimeValue());
        RealStopwatch(const ::Inscription::Table<RealStopwatch>& table);

        bool operator==(const RealStopwatch& arg) const;

        TimeValue CurrentTime() const override;

        ObjectTypeDescription TypeDescription() const override;
    private:
        TimeSystem* timeSystem;
    private:
        INSCRIPTION_ACCESS;
    };

    template<>
    struct ObjectTraits<RealStopwatch> : ObjectTraitsBase<RealStopwatch>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<StopwatchBase> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::RealStopwatch)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}