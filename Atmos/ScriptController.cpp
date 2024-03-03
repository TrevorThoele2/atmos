
#include "ScriptController.h"

#include "ScriptAsset.h"
#include "AngelScriptUtility.h"
#include "AngelScriptAssert.h"
#include <angelscript.h>

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"
#include "LoggingSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    ScriptController::ScriptController(ObjectManager& manager) : ObjectSystem(manager)
    {
        runningScripts = manager.Batch<RunningScript>();
        runningScripts.onCreated.Subscribe(&ScriptController::OnRunningScriptCreated, *this);
    }

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(ScriptController) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

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
                Scripting::ParameterIndex parameterIndex = 0;
                for (auto& loop : (*itr)->parameters)
                    Scripting::PushVariantToAngelScriptParameter(parameterIndex, loop, context);

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

            Scripting::AngelScriptAssert(context->Prepare(func));
        }

        Scripting::AngelScriptAssert(context->Execute());

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
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::ScriptController)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::ScriptController, "ScriptController");
}