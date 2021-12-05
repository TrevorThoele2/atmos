#include "JavaScriptScriptResource.h"

#include "ScriptError.h"

namespace Atmos::Scripting::JavaScript
{
    ScriptResource::ScriptResource(std::weak_ptr<Module> module, const Parameters& parameters, const String& executeFunctionName) :
        executeFunctionName(executeFunctionName),
        parameters(parameters),
        module(module)
    {}

    ScriptResource::~ScriptResource()
    {
        const auto module = this->module.lock();
        if (module)
            Module::Destroy(*module);
    }

    std::optional<Scripting::Result> ScriptResource::Execute()
    {
        module.lock()->Instantiate();
        return DoExecute();
    }

    std::optional<Scripting::Result> ScriptResource::Resume()
    {
        return DoExecute();
    }

    void ScriptResource::Suspend()
    {

    }

    std::optional<Scripting::Result> ScriptResource::DoExecute()
    {
    	return module.lock()->Execute(parameters, executeFunctionName);
    }
}