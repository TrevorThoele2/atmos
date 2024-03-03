
#include "ScriptInstance.h"

#include "RunningScript.h"
#include "ScriptController.h"

#include "ObjectManager.h"

namespace Atmos
{
    ScriptInstance::ScriptInstance(ObjectManager& manager) : Object(manager)
    {}

    ScriptInstance::ScriptInstance(const ::Inscription::BinaryTableData<ScriptInstance>& data) :
        Object(std::get<0>(data.bases))
    {}

    void ScriptInstance::ExecuteDeferred()
    {
        if (IsRunning())
            return;

        CreateRunningFromThis();
    }

    void ScriptInstance::ExecuteImmediately()
    {
        if (IsRunning())
            return;

        auto running = CreateRunningFromThis();
        Manager()->FindSystem<ScriptController>()->ExecuteImmediately(running);
    }

    TypedObjectReference<RunningScript> ScriptInstance::RunningForThis() const
    {
        if (!IsRunning())
            return TypedObjectReference<RunningScript>();

        return Manager()->FindSystem<ScriptController>()->RunningScriptFor(*this);
    }

    bool ScriptInstance::IsRunning() const
    {
        return Manager()->FindSystem<ScriptController>()->IsRunning(this);
    }

    ObjectTypeDescription ScriptInstance::TypeDescription() const
    {
        return ObjectTraits<ScriptInstance>::TypeDescription();
    }

    TypedObjectReference<RunningScript> ScriptInstance::CreateRunningFromThis()
    {
        auto running = Manager()->CreateObject<RunningScript>(*this);
        running->owner = owner;
        running->executeName = executeName;
        running->parameters = parameters;
        running->persistence = persistence;
        return running;
    }

    const ObjectTypeName ObjectTraits<ScriptInstance>::typeName = "ScriptInstance";
}

namespace Inscription
{
    Scribe<::Atmos::ScriptInstance, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::asset, &DataT::asset),
            DataEntry::Auto(&ObjectT::executeName, &DataT::executeName),
            DataEntry::Auto(&ObjectT::parameters, &DataT::parameters),
            DataEntry::Auto(&ObjectT::persistence, &DataT::persistence) });
    }
}