#include "AngelScriptScriptResource.h"

#include <angelscript.h>
#include "AngelScriptResultVerification.h"
#include "AngelScriptFunctionManagement.h"

#include "ScriptFinished.h"

namespace Atmos::Scripting::Angel
{
    ScriptResource::ScriptResource(
        Parameters parameters,
        asIScriptFunction& executeFunction,
        asIScriptContext& context,
        Logging::Logger& logger)
        :
        parameters(parameters),
        executeFunction(&executeFunction),
        context(&context),
        logger(&logger)
    {
        executeFunction.AddRef();
    }

    ScriptResource::~ScriptResource()
    {
        for (auto& group : groups)
            for(auto& function : group.second.angelScriptFunctions)
                function->Release();

        executeFunction->Release();
        context->GetEngine()->ReturnContext(context);
    }

    std::optional<Result> ScriptResource::Execute()
    {
        executionType = ExecutionType::Executing;

        for(auto& group : groups)
        {
            for (auto secondaryExecution = group.second.secondaryExecutions.begin(); secondaryExecution != group.second.secondaryExecutions.end();)
            {
                DoExecuteSecondary(*secondaryExecution, group.second.angelScriptFunctions, *context, *logger);
                secondaryExecution = group.second.secondaryExecutions.erase(secondaryExecution);
            }
        }

        VerifyResult(context->Prepare(executeFunction));

        auto index = 0;
        for (auto& parameter : parameters)
        {
            PushVariantToParameter(index, parameter, *context);
            ++index;
        }

        return DoExecute(*executeFunction, *context);
    }

    std::optional<Result> ScriptResource::Resume()
    {
        executionType = ExecutionType::Executing;

        VerifyResult(context->Prepare(executeFunction));
        return DoExecute(*executeFunction, *context);
    }

    void ScriptResource::Suspend()
    {
        executionType = ExecutionType::Suspended;

        VerifyResult(context->Suspend());
    }

    void ScriptResource::StoreFunction(const String& name, asIScriptFunction& function)
    {
        auto group = groups.find(name);
        if (group == groups.end())
            group = groups.emplace(name, Group{}).first;
        group->second.angelScriptFunctions.push_back(&function);
    }

    void ScriptResource::ExecuteSecondaryAgainstStoredFunctions(const String& name, const SecondaryExecution& againstContext)
    {
        auto group = groups.find(name);
        if (group == groups.end())
            return;

        if (executionType == ExecutionType::Executing)
            DoExecuteSecondary(againstContext, group->second.angelScriptFunctions, *context, *logger);
        else
            group->second.secondaryExecutions.push_back(againstContext);
    }

    std::optional<Result> ScriptResource::DoExecute(asIScriptFunction& function, asIScriptContext& context)
    {
        const auto result = context.Execute();
        if (result == asEXECUTION_FINISHED)
            return Result{ PullVariantFromReturn(context) };
        else if (result == asEXECUTION_EXCEPTION)
        {
            const String exceptionMessage = context.GetExceptionString();
            const auto exceptionFunction = context.GetExceptionFunction();
            const auto functionName = exceptionFunction
                ? std::optional<String>{ String(exceptionFunction->GetName()) }
                : std::optional<String>{};

            int column;
            const char* section;
            const auto line = context.GetExceptionLineNumber(&column, &section);
            const auto sectionName = section
                ? std::optional<String>{ String(section) }
            : std::optional<String>{};

            return Result{ Failure { exceptionMessage, functionName, line, column, sectionName } };
        }
        else
            return {};
    }

    void ScriptResource::DoExecuteSecondary(
        const SecondaryExecution& secondaryExecution,
        AngelScriptFunctions& angelScriptFunctions,
        asIScriptContext& context,
        Logging::Logger& logger)
    {
        for (auto& storedFunction : angelScriptFunctions)
        {
            if (IsError(context.PushState()))
                continue;

            VerifyResult(context.Prepare(storedFunction));

            secondaryExecution(context);
            auto result = DoExecute(*storedFunction, context);
            if (result && std::holds_alternative<Failure>(*result) == 1)
            {
                const auto failure = std::get<Failure>(*result);
                logger.Log(ToLog(failure));
            }
            context.PopState();
        }
    }
}