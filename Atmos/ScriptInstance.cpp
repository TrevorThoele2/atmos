
#include "ScriptInstance.h"

#include "RunningScript.h"
#include "ScriptController.h"

#include "ObjectManager.h"

namespace Atmos
{
    ScriptInstance::ScriptInstance(ObjectManager& manager) : Object(manager)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(ScriptInstance) : INSCRIPTION_TABLE_GET_BASE(Object)
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

    const ObjectTypeName ObjectTraits<ScriptInstance>::typeName = "ScriptInstance";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::ScriptInstance)
    {
        INSCRIPTION_TABLE_ADD(asset);

        INSCRIPTION_TABLE_ADD(executeName);
        INSCRIPTION_TABLE_ADD(parameters);

        INSCRIPTION_TABLE_ADD(globalItems);
        INSCRIPTION_TABLE_ADD(persistence);
    }
}