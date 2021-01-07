#include "AngelScriptScriptResource.h"

#include <angelscript.h>
#include "AngelScriptResultVerification.h"
#include "AngelScriptFunctionManagement.h"

#include "ScriptFinished.h"

#include "ActionPressed.h"

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

    ScriptResource::ScriptResource(ScriptResource&& arg) noexcept :
        parameters(std::move(arg.parameters)),
        executeFunction(arg.executeFunction),
        context(arg.context),
        executionType(arg.executionType),
        groups(std::move(arg.groups)),
        logger(arg.logger)
    {
        arg.executeFunction = nullptr;
        arg.context = nullptr;
        arg.logger = nullptr;
    }

    ScriptResource::~ScriptResource()
    {
        for (auto& group : groups)
            for(auto& function : group.second.angelScriptFunctions)
                function->Release();

        if (executeFunction)
            executeFunction->Release();
        if (context)
            context->GetEngine()->ReturnContext(context);
    }

    ScriptResource& ScriptResource::operator=(ScriptResource&& arg) noexcept
    {
        parameters = std::move(arg.parameters);
        executeFunction = arg.executeFunction;
        context = arg.context;
        executionType = arg.executionType;
        groups = std::move(arg.groups);
        logger = arg.logger;

        arg.executeFunction = nullptr;
        arg.context = nullptr;
        arg.logger = nullptr;

        return *this;
    }

    std::optional<Result> ScriptResource::Execute()
    {
        executionType = ExecutionType::Executing;

        ExecuteAllStoredSecondaries();

        VerifyResult(context->Prepare(executeFunction));

        auto index = 0;
        for (auto& parameter : parameters)
        {
            PushVariantToParameter(index, parameter, *context);
            ++index;
        }

        return DoExecute(*context);
    }

    std::optional<Result> ScriptResource::Resume()
    {
        executionType = ExecutionType::Executing;

        ExecuteAllStoredSecondaries();

        return DoExecute(*context);
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
            DoExecuteSecondary(againstContext, group->second.angelScriptFunctions, *context->GetEngine(), *logger);
        else
            group->second.secondaryExecutions.push_back(againstContext);
    }

    std::optional<Result> ScriptResource::DoExecute(asIScriptContext& context)
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

    void ScriptResource::ExecuteAllStoredSecondaries()
    {
        for (auto& group : groups)
        {
            for (auto secondaryExecution = group.second.secondaryExecutions.begin(); secondaryExecution != group.second.secondaryExecutions.end();)
            {
                DoExecuteSecondary(*secondaryExecution, group.second.angelScriptFunctions, *context->GetEngine(), *logger);
                secondaryExecution = group.second.secondaryExecutions.erase(secondaryExecution);
            }
        }
    }

    void ScriptResource::DoExecuteSecondary(
        const SecondaryExecution& secondaryExecution,
        AngelScriptFunctions& angelScriptFunctions,
        asIScriptEngine& engine,
        Logging::Logger& logger)
    {
        for (auto& storedFunction : angelScriptFunctions)
        {
            auto newContext = engine.RequestContext();

            VerifyResult(newContext->Prepare(storedFunction));

            secondaryExecution(*newContext);
            auto result = DoExecute(*newContext);
            if (result && std::holds_alternative<Failure>(*result) == 1)
            {
                const auto failure = std::get<Failure>(*result);
                logger.Log(ToLog(failure));
            }

            engine.ReturnContext(newContext);
        }
    }
}