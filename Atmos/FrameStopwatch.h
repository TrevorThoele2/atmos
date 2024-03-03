#pragma once

#include "StopwatchBase.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class TimeSystem;

    class FrameStopwatch : public StopwatchBase
    {
    public:
        FrameStopwatch(ObjectManager& manager, TimeValue goal = TimeValue());
        FrameStopwatch(const ::Inscription::Table<FrameStopwatch>& table);

        bool operator==(const FrameStopwatch& arg) const;
        bool operator!=(const FrameStopwatch& arg) const;

        TimeValue CurrentTime() const override;

        ObjectTypeDescription TypeDescription() const override;
    private:
        TimeSystem* timeSystem;
    private:
        INSCRIPTION_ACCESS;
    };

    template<>
    struct ObjectTraits<FrameStopwatch> : ObjectTraitsBase<FrameStopwatch>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<StopwatchBase> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::FrameStopwatch)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}