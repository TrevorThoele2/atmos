#include "ScriptInstance.h"

#include <Arca/Reliquary.h>

#include "RunningScript.h"
#include "AllRunningScripts.h"
#include "ExecuteImmediately.h"

namespace Atmos::Script
{
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

        auto& running = CreateRunningFromThis();
        Owner().Raise<Script::ExecuteImmediately>(running);
    }

    RunningScript* ScriptInstance::RunningForThis() const
    {
        if (!IsRunning())
            return nullptr;

        return Arca::GlobalIndex<AllRunningScripts>(Owner())->RunningScriptFor(*this);
    }

    bool ScriptInstance::IsRunning() const
    {
        return Arca::GlobalIndex<AllRunningScripts>(Owner())->IsRunning(*this);
    }

    RunningScript& ScriptInstance::CreateRunningFromThis()
    {
        auto running = Owner().Create<RunningScript>(*this);
        running->executeName = executeName;
        running->parameters = parameters;
        running->persistence = persistence;
        return *running;
    }
}