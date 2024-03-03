#pragma once

#include "ScriptAssetResource.h"

class asIScriptModule;

namespace Atmos::Scripting::Angel::Resource
{
    class Script final : public Asset::Resource::Script
    {
    public:
        explicit Script(asIScriptModule& module);

        [[nodiscard]] asIScriptModule* Module() const;
    private:
        asIScriptModule* module = nullptr;
    };
}