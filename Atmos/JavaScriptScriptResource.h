#pragma once

#include "ScriptResource.h"
#include "ScriptParameters.h"
#include "JavaScriptModule.h"

namespace Atmos::Scripting::JavaScript
{
    class ScriptResource final : public Atmos::Scripting::Resource
    {
    public:
        explicit ScriptResource(
            std::weak_ptr<Module> module, const Parameters& parameters, const String& executeFunctionName);
        ~ScriptResource();

        std::optional<Scripting::Result> Execute() override;

        std::optional<Scripting::Result> Resume() override;
        void Suspend() override;
    private:
        String executeFunctionName;
        Parameters parameters;

        std::weak_ptr<Module> module;

        std::optional<Scripting::Result> DoExecute();
    };
}