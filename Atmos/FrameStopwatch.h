#pragma once

#include "Stopwatch.h"

#include "ObjectScribe.h"

namespace Atmos
{
    class TimeSystem;

    class FrameStopwatch : public Stopwatch
    {
    public:
        FrameStopwatch(ObjectManager& manager, TimeValue goal = TimeValue());
        FrameStopwatch(const ::Inscription::BinaryTableData<FrameStopwatch>& data);

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
        static constexpr ObjectTypeList<Stopwatch> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::FrameStopwatch, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::FrameStopwatch, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::FrameStopwatch, BinaryArchive> :
        public ObjectScribe<::Atmos::FrameStopwatch, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}