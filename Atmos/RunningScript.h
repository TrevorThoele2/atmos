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

namespace Atmos
{
    class RunningScript : public Object
    {
    public:
        typedef TypedObjectReference<ScriptInstance> SourceReference;
    public:
        using SourceProperty = ReadonlyProperty<SourceReference>;
        SourceProperty source;

        Name executeName;
        Scripting::Parameters parameters;
        ObjectReference owner;

        Scripting::Persistence persistence;
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

    template<>
    struct ObjectTraits<RunningScript> : ObjectTraitsBase<RunningScript>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::RunningScript, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::RunningScript, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::RunningScript, BinaryArchive> : public ObjectScribe<::Atmos::RunningScript, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}