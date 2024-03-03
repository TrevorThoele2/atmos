#pragma once

#include "ScriptAssetResource.h"
#include "String.h"
#include "JavaScriptModule.h"

namespace Atmos::Scripting::JavaScript::Resource
{
    class ScriptAsset final : public Asset::Resource::Script
    {
    public:
        explicit ScriptAsset(const String& source, std::weak_ptr<Module> module);
        ~ScriptAsset();

        [[nodiscard]] String Source() const;
    private:
        String source;

        std::weak_ptr<Module> module;
    };
}