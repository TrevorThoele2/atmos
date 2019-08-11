#include "ScriptController.h"

#include "ScriptAsset.h"
#include "AngelScriptUtility.h"
#include "AngelScriptAssert.h"
#include <angelscript.h>

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"
#include "LoggingSystem.h"

#include "StringUtility.h"

namespace Atmos::Script
{
    ScriptController::ScriptController(ObjectManager& manager) : ObjectSystem(manager)
    {
        runningScripts = manager.Batch<RunningScript>();
        runningScripts.onCreated.Subscribe(&ScriptController::OnRunningScriptCreated, *this);
    }

    void ScriptController::ExecuteImmediately(RunningScriptReference reference)
    {
        LaunchOrRunScript(Find(reference));
    }

    void ScriptController::ForceQuit(RunningScriptReference reference)
    {

    }

    ScriptController::RunningScriptReference ScriptController::Current()
    {
        if (current == runningScripts.end())
            return RunningScriptReference();

        return *current;
    }

    ObjectBatchSizeT ScriptController::Size() const
    {
        return runningScripts.Size();
    }

    ScriptController::RunningScriptReference ScriptController::RunningScriptFor(ScriptInstanceReference scriptInstance) const
    {
        auto& found = runningScriptMap.find(scriptInstance);
        if (found == runningScriptMap.end())
            return RunningScriptReference();

        return found->second;
    }

    bool ScriptController::IsRunning(ScriptInstanceReference scriptInstance) const
    {
        return RunningScriptFor(scriptInstance).IsOccupied();
    }

    void ScriptController::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.workingScriptSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }

    void ScriptController::WorkImpl()
    {
        for (auto& loop : runningScripts)
            loop->Resume();

        // Resume all of the stack
        current = runningScripts.begin();
        while (current != runningScripts.end())
        {
            auto next = current;
            ++next;

            if (!(*current)->executedThisFrame && !(*current)->IsSuspended())
            {
                LaunchOrRunScript(current);
                (*current)->executedThisFrame = true;
            }

            current = next;
        }

        current = runningScripts.end();

        for (auto loop = runningScripts.begin(); loop != runningScripts.end();)
        {
            (*loop)->executedThisFrame = false;
            if (!(*loop)->IsSuspended())
                loop = runningScripts.RemoveObject(loop);
            else
                ++loop;
        }
    }

    ScriptController::RunningIterator ScriptController::Find(RunningScriptReference reference)
    {
        for (auto loop = runningScripts.begin(); loop != runningScripts.end(); ++loop)
            if (*loop == reference)
                return loop;

        return runningScripts.end();
    }

    void ScriptController::Remove(RunningIterator itr)
    {
        if (current == itr)
            current = runningScripts.RemoveObject(itr);
        else
            runningScripts.RemoveObject(itr);
    }

    void ScriptController::LaunchOrRunScript(RunningIterator itr)
    {
        if ((*itr)->executedThisFrame)
            return;

        bool error = false;
        asIScriptContext* context = (*itr)->UnderlyingContext();

        if (!(*itr)->hasBeenExecuted)
        {
            (*itr)->hasBeenExecuted = true;

            const asIScriptModule* mod = (*itr)->source->asset->UnderlyingModule();

            asIScriptFunction* func = nullptr;
            if ((*itr)->ShouldExecuteMain())
                func = mod->GetFunctionByDecl("void Main()");
            else
            {
                ParameterIndex parameterIndex = 0;
                for (auto& loop : (*itr)->parameters)
                    PushVariantToAngelScriptParameter(parameterIndex, loop, context);

                func = mod->GetFunctionByDecl((*itr)->executeName.c_str());
            }

            if (!func)
            {
                FindLoggingSystem()->Log(String("Cannot find the function to execute for a script.\n"),
                    LogType::ERROR_SEVERE,
                    LogNameValueVector{
                        NameValuePair("Script File Name", (*itr)->source->asset->fileName.GetValue()),
                        NameValuePair("Execute Name", (*itr)->executeName) });
            }

            AngelScriptAssert(context->Prepare(func));
        }

        AngelScriptAssert(context->Execute());

        (*itr)->executedThisFrame = true;

        if (!error)
        {
            if (!(*itr)->IsSuspended())
                Remove(itr);
        }
        else
            Remove(itr);
    }

    void ScriptController::OnRunningScriptCreated(RunningScriptReference reference)
    {
        runningScriptMap.emplace(reference->source.Get(), reference);
    }

    void ScriptController::OnRunningScriptDestroyed(RunningScriptReference reference)
    {
        runningScriptMap.erase(reference->source.Get());
    }

    LoggingSystem* ScriptController::FindLoggingSystem()
    {
        return Manager()->FindSystem<LoggingSystem>();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Script::ScriptController, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}