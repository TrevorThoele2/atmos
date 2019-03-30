#pragma once

#include "Object.h"
#include "ObjectReference.h"

#include "ScriptAsset.h"
#include "ScriptGlobalItems.h"
#include "ScriptParameters.h"
#include "ScriptPersistence.h"

#include "StoredProperty.h"
#include "ReadonlyProperty.h"
#include "StoredReadonlyProperty.h"

#include "NameValuePair.h"

#include "ObjectSerialization.h"

namespace Falcon
{
    class Item;
}

namespace Atmos
{
    class RunningScript;

    class ScriptInstance : public Object
    {
    public:
        typedef TypedObjectReference<ScriptAsset> AssetReference;
    public:
        StoredProperty<AssetReference> asset;

        ObjectReference owner;

        Name executeName;
        Scripting::Parameters parameters;

        Scripting::GlobalItems globalItems;
        Scripting::Persistence persistence;
    public:
        ScriptInstance();
        ScriptInstance(const ScriptInstance& arg) = default;
        ScriptInstance(const ::Inscription::Table<ScriptInstance>& table);

        // Executes the script standardly
        // Will defer the execute until a certain point in the frame (probably a bit later)
        void ExecuteDeferred();
        // Executes the script standardly
        // Will immediately execute the script
        void ExecuteImmediately();
        TypedObjectReference<RunningScript> RunningForThis() const;
        // Returns if this script is currently running
        // NOTE: Suspension has no effect on this
        bool IsRunning() const;

        ObjectTypeDescription TypeDescription() const override;
    private:
        TypedObjectReference<RunningScript> CreateRunningFromThis();
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<>
    struct ObjectTraits<ScriptInstance> : ObjectTraitsBase<ScriptInstance>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::ScriptInstance)
    {
    public:
        static void AddMembers(TableT& table);
    };
}