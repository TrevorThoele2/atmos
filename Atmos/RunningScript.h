#pragma once

#include <memory>

#include "Object.h"
#include "ObjectReference.h"

#include "ScriptInstance.h"
#include "ScriptGlobalItems.h"
#include "ScriptParameters.h"
#include "ScriptPersistence.h"

#include "ReadonlyProperty.h"

#include "ObjectSerialization.h"

class asIScriptContext;

namespace Atmos
{
    class RunningScript : public Object
    {
    public:
        typedef TypedObjectReference<ScriptInstance> SourceReference;
    public:
        ReadonlyProperty<SourceReference> source;

        Name executeName;
        Scripting::Parameters parameters;
        ObjectReference owner;

        Scripting::GlobalItems globalItems;
        Scripting::Persistence persistence;
    public:
        bool hasBeenExecuted;
        bool executedThisFrame;
    private:
        class Data;
    public:
        RunningScript(ObjectManager& manager, SourceReference source);
        RunningScript(const RunningScript& arg) = default;
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(RunningScript);

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
    DECLARE_OBJECT_INSCRIPTER(::Atmos::RunningScript)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}