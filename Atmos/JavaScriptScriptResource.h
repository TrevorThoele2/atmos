#pragma once

#include "ScriptResource.h"
#include "ScriptParameters.h"
#include "JavaScriptModule.h"

namespace Atmos::Scripting::JavaScript
{
    class ScriptResource final : public Resource
    {
    public:
        explicit ScriptResource(
            std::weak_ptr<Module> module, const Parameters& parameters, const String& executeFunctionName);
        ~ScriptResource();

        std::optional<Result> Execute() override;

        std::optional<Result> Resume() override;
        void Suspend() override;
    private:
        String executeFunctionName;
        Parameters parameters;

        std::weak_ptr<Module> module;

        std::optional<Result> DoExecute();
    };
}