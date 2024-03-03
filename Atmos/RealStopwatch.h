#pragma once

#include "Stopwatch.h"

#include "ObjectScribe.h"

namespace Atmos
{
    class TimeSystem;

    class RealStopwatch : public Stopwatch
    {
    public:
        RealStopwatch(ObjectManager& manager, TimeValue goal = TimeValue());
        RealStopwatch(const ::Inscription::BinaryTableData<RealStopwatch>& data);

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
        static constexpr ObjectTypeList<Stopwatch> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::RealStopwatch, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::RealStopwatch, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::RealStopwatch, BinaryArchive> : public ObjectScribe<::Atmos::RealStopwatch, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}