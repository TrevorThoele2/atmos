#pragma once

#include "Field.h"
#include "FieldID.h"
#include "Optional.h"

namespace inscription
{
    class Scribe;
}

namespace Atmos
{
    class ScribeBase
    {
    protected:
        virtual inscription::Scribe& GetBasicScribe() = 0;
        virtual const inscription::Scribe& GetBasicScribe() const = 0;
    public:
        virtual ~ScribeBase() = 0 {}
        void CopyTrackersTo(ScribeBase &arg) const;
        void CopyTrackersTo(inscription::Scribe &scribe) const;
        void MoveTrackersTo(ScribeBase &arg);
        void MoveTrackersTo(inscription::Scribe &scribe);
    };

    class OutScribeBase : public ScribeBase
    {
    public:
        virtual ~OutScribeBase() = 0 {}
        virtual void Save(Field &field) = 0;
        virtual void OverwriteFieldCount(inscription::ContainerSize::ValueT set) = 0;
    };

    class WorldStartBase;
    class InScribeBase : public ScribeBase
    {
    public:
        enum LoadType
        {
            LOAD_ALL,
            LOAD_GLOBAL,
            LOAD_FIELD_PLACEHOLDERS
        };
    public:
        virtual ~InScribeBase() = 0 {}
        // Returns if the load was successful
        virtual bool Load(LoadType type = LOAD_ALL) = 0;
        virtual bool WillLoad() = 0;

        virtual Optional<Field> Get(FieldID id) = 0;
        virtual Field* GetAsHeap(FieldID id) = 0;
        virtual inscription::Buffer GetAsBuffer(FieldID id) = 0;

        virtual WorldStartBase& GetWorldStart() = 0;

        virtual void GetIDs(std::vector<FieldID> &ids) const = 0;
        virtual bool HasField(FieldID fieldID) const = 0;
        virtual size_t GetFieldCount() const = 0;
    };
}