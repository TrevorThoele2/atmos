#pragma once

#include "ScriptResource.h"
#include "ScriptParameters.h"
#include <angelscript.h>
#include "AngelScriptResultVerification.h"
#include "Logger.h"

namespace Atmos::Scripting::Angel
{
    class ScriptResource final : public Atmos::Scripting::Resource
    {
    public:
        using SecondaryExecution = std::function<void(asIScriptContext&)>;
    public:
        explicit ScriptResource(
            Parameters parameters,
            asIScriptFunction& executeFunction,
            asIScriptContext& context,
            Logging::Logger& logger);
        ~ScriptResource();

        std::optional<Result> Execute() override;

        std::optional<Result> Resume() override;
        void Suspend() override;
    public:
        void StoreFunction(const String& name, asIScriptFunction& function);
        void ExecuteSecondaryAgainstStoredFunctions(const String& name, const SecondaryExecution& againstContext);
    private:
        Parameters parameters;
        static std::optional<Result> DoExecute(asIScriptFunction& function, asIScriptContext& context);
    private:
        asIScriptFunction* executeFunction = nullptr;
        asIScriptContext* context = nullptr;
    private:
        enum class ExecutionType
        {
            Executing,
            Suspended
        };
        ExecutionType executionType = ExecutionType::Suspended;
    private:
        using AngelScriptFunctions = std::vector<asIScriptFunction*>;

        struct Group
        {
            AngelScriptFunctions angelScriptFunctions;
            std::vector<SecondaryExecution> secondaryExecutions;
        };

        std::unordered_map<String, Group> groups;

        void ExecuteAllStoredSecondaries();
        static void DoExecuteSecondary(
            const SecondaryExecution& secondaryExecution,
            AngelScriptFunctions& angelScriptFunctions,
            asIScriptEngine& engine,
            Logging::Logger& logger);
    private:
        Logging::Logger* logger = nullptr;
    };
}