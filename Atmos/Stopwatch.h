#pragma once

#include "Object.h"

#include "TimeValue.h"

#include "ObjectScribe.h"

namespace Atmos::Time
{
    class Stopwatch : public Object
    {
    public:
        Stopwatch(ObjectManager& manager, Value goal = Value());
        Stopwatch(const ::Inscription::BinaryTableData<Stopwatch>& data);

        virtual ~Stopwatch() = 0;

        bool operator==(const Stopwatch& arg) const;
        bool operator!=(const Stopwatch& arg) const;

        Value Start();
        void SetStart(const Value& set);
        Value GetStart() const;

        void SetGoal(Value set);
        Value GetGoal() const;
        bool HasReachedGoal() const;

        Value Elapsed() const;
        // Instead of 0% - 100%, it's 0 - 1 (but you probably already knew that didn't you?)
        Value PercentageElapsed() const;

        virtual Value CurrentTime() const = 0;

        ObjectTypeDescription TypeDescription() const override;
    private:
        Value start;
        Value goal;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Time::Stopwatch> : ObjectTraitsBase<Time::Stopwatch>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Time::Stopwatch, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Time::Stopwatch, BinaryArchive>
    {
        ::Atmos::Time::Value goal;
    };

    template<>
    class Scribe<::Atmos::Time::Stopwatch, BinaryArchive> :
        public ObjectScribe<::Atmos::Time::Stopwatch, BinaryArchive>
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