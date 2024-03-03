#pragma once

#include <memory>

#include "Object.h"
#include "ObjectReference.h"

#include "ScriptInstance.h"
#include "ScriptParameters.h"
#include "ScriptPersistence.h"

#include "ReadonlyProperty.h"

#include "ObjectScribe.h"

class asIScriptContext;

namespace Atmos::Script
{
    class RunningScript : public Object
    {
    public:
        typedef TypedObjectReference<ScriptInstance> SourceReference;
    public:
        using SourceProperty = ReadonlyProperty<SourceReference>;
        SourceProperty source;

        Name executeName;
        Parameters parameters;
        ObjectReference owner;

        Persistence persistence;
    public:
        bool hasBeenExecuted;
        bool executedThisFrame;
    private:
        class Data;
    public:
        RunningScript(ObjectManager& manager, SourceReference source);
        RunningScript(const RunningScript& arg) = default;
        RunningScript(const ::Inscription::BinaryTableData<RunningScript>& data);

        void Resume();
        void Suspend();
        bool IsSuspended() const;

        bool ShouldExecuteMain() const;

        asIScriptContext* UnderlyingContext();

        ObjectTypeDescription TypeDescription() const override;
    private:
        std::unique_ptr<Data> data;
        bool suspended;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Script::RunningScript> : ObjectTraitsBase<Script::RunningScript>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Script::RunningScript, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Script::RunningScript, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Script::RunningScript, BinaryArchive> :
        public ObjectScribe<::Atmos::Script::RunningScript, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}