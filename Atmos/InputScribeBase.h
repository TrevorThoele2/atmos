#pragma once

#include <vector>

#include "ScribeBase.h"

#include "WorldStart.h"

#include "Field.h"
#include "FieldID.h"
#include "Optional.h"

namespace Atmos
{
    class InputScribeBase : public ScribeBase
    {
    public:
        enum LoadType
        {
            LOAD_ALL,
            LOAD_GLOBAL,
            LOAD_FIELD_PLACEHOLDERS
        };
    public:
        typedef std::vector<FieldID> FieldIDList;
    public:
        typedef ReadonlyProperty<WorldStart> WorldStartProperty;
        WorldStartProperty worldStart;
    public:
        virtual ~InputScribeBase() = 0;

        // Returns if the load was successful
        virtual bool Load(LoadType type = LOAD_ALL) = 0;
        virtual bool WillLoad() = 0;

        virtual Optional<Field> ExtractField(FieldID id) = 0;
        virtual std::unique_ptr<Field> ExtractFieldAsHeap(FieldID id) = 0;
        virtual ::Inscription::Buffer ExtractFieldAsBuffer(FieldID id) = 0;

        virtual FieldIDList AllFieldIDs() const = 0;
        virtual bool HasField(FieldID fieldID) const = 0;
        virtual size_t FieldCount() const = 0;
    protected:
        InputScribeBase(WorldStartProperty::Getter worldStartGetter);
    };
}