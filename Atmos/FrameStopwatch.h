#pragma once

#include "Stopwatch.h"

#include "ObjectScribe.h"

namespace Atmos::Time
{
    class TimeSystem;

    class FrameStopwatch : public Stopwatch
    {
    public:
        FrameStopwatch(ObjectManager& manager, Value goal = Value());
        FrameStopwatch(const ::Inscription::BinaryTableData<FrameStopwatch>& data);

        bool operator==(const FrameStopwatch& arg) const;
        bool operator!=(const FrameStopwatch& arg) const;

        Value CurrentTime() const override;

        ObjectTypeDescription TypeDescription() const override;
    private:
        TimeSystem* timeSystem;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Time::FrameStopwatch> : ObjectTraitsBase<Time::FrameStopwatch>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Time::Stopwatch> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Time::FrameStopwatch, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Time::FrameStopwatch, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Time::FrameStopwatch, BinaryArchive> :
        public ObjectScribe<::Atmos::Time::FrameStopwatch, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}