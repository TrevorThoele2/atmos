
#include "ScriptInstance.h"

#include "RunningScript.h"
#include "ScriptController.h"

#include "ObjectManager.h"

#include "StasisScribe.h"

namespace Atmos
{
    ScriptInstance::ScriptInstance()
    {}

    ScriptInstance::ScriptInstance(const ::Inscription::Table<ScriptInstance>& table) : INSCRIPTION_TABLE_GET_BASE(Object)
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
        running->globalItems = globalItems;
        running->persistence = persistence;
        return running;
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ScriptInstance)
    {
        scribe(asset);

        scribe(executeName);
        scribe(parameters);

        scribe(globalItems);
        scribe(persistence);
    }

    const ObjectTypeName ObjectTraits<ScriptInstance>::typeName = "ScriptInstance";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::ScriptInstance)
    {

    }
}