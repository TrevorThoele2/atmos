#include "JavaScriptAssetResource.h"

namespace Atmos::Scripting::JavaScript::Resource
{
    ScriptAsset::ScriptAsset(const String& source, std::weak_ptr<Module> module) :
        source(source), module(module)
    {}

    ScriptAsset::~ScriptAsset()
    {
        const auto module = this->module.lock();
        if (module)
			Module::Destroy(*module);
    }

    String ScriptAsset::Source() const
    {
        return source;
    }
}