#pragma once

#include "Object.h"
#include "ObjectReference.h"

#include "ScriptAsset.h"
#include "ScriptParameters.h"
#include "ScriptPersistence.h"

#include "StoredProperty.h"
#include "ReadonlyProperty.h"
#include "StoredReadonlyProperty.h"

#include "NameValuePair.h"

#include "ObjectScribe.h"

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

        Scripting::Persistence persistence;
    public:
        ScriptInstance(ObjectManager& manager);
        ScriptInstance(const ScriptInstance& arg) = default;
        ScriptInstance(const ::Inscription::BinaryTableData<ScriptInstance>& data);

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
    };

    template<>
    struct ObjectTraits<ScriptInstance> : ObjectTraitsBase<ScriptInstance>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::ScriptInstance, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::ScriptInstance, BinaryArchive>
    {
        ObjectT::AssetReference asset;
        ::Atmos::Name executeName;
        ::Atmos::Scripting::Parameters parameters;
        ::Atmos::Scripting::Persistence persistence;
    };

    template<>
    class Scribe<::Atmos::ScriptInstance, BinaryArchive> : public ObjectScribe<::Atmos::ScriptInstance, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };
}