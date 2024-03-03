#pragma once

#include "Object.h"

#include "TimeValue.h"

#include "ObjectScribe.h"

namespace Atmos
{
    class Stopwatch : public Object
    {
    public:
        Stopwatch(ObjectManager& manager, TimeValue goal = TimeValue());
        Stopwatch(const ::Inscription::BinaryTableData<Stopwatch>& data);

        virtual ~Stopwatch() = 0;

        bool operator==(const Stopwatch& arg) const;
        bool operator!=(const Stopwatch& arg) const;

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
    struct ObjectTraits<Stopwatch> : ObjectTraitsBase<Stopwatch>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Stopwatch, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Stopwatch, BinaryArchive>
    {
        ::Atmos::TimeValue goal;
    };

    template<>
    class Scribe<::Atmos::Stopwatch, BinaryArchive> : public ObjectScribe<::Atmos::Stopwatch, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    };
}