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

        auto running = CreateRunningFromThis();
        Owner().Do<Script::ExecuteImmediately>(running);
    }

    Arca::RelicIndex<RunningScript> ScriptInstance::RunningForThis() const
    {
        if (!IsRunning())
            return {};

        return Arca::GlobalIndex<AllRunningScripts>(Owner())->RunningScriptFor(ID());
    }

    bool ScriptInstance::IsRunning() const
    {
        return Arca::GlobalIndex<AllRunningScripts>(Owner())->IsRunning(ID());
    }

    Arca::RelicIndex<RunningScript> ScriptInstance::CreateRunningFromThis()
    {
        return Owner().Do<Arca::Create<RunningScript>>(
            Arca::RelicIndex<ScriptInstance>(ID(), Owner()),
            executeName,
            parameters,
            persistence);
    }
}