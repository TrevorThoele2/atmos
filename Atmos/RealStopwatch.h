#pragma once

#include "Stopwatch.h"

#include "ObjectScribe.h"

namespace Atmos::Time
{
    class TimeSystem;

    class RealStopwatch : public Stopwatch
    {
    public:
        RealStopwatch(ObjectManager& manager, Value goal = Value());
        RealStopwatch(const ::Inscription::BinaryTableData<RealStopwatch>& data);

        bool operator==(const RealStopwatch& arg) const;

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
    struct ObjectTraits<Time::RealStopwatch> : ObjectTraitsBase<Time::RealStopwatch>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Time::Stopwatch> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Time::RealStopwatch, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Time::RealStopwatch, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Time::RealStopwatch, BinaryArchive> :
        public ObjectScribe<::Atmos::Time::RealStopwatch, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}