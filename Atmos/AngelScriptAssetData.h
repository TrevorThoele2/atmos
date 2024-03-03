#pragma once

#include "ScriptAssetResource.h"
#include <angelscript.h>

namespace Atmos::Script::Angel
{
    class AssetDataImplementation final : public Asset::Resource::Script
    {
    public:
        explicit AssetDataImplementation(asIScriptModule& module);

        [[nodiscard]] asIScriptModule* Module() const;
    private:
        asIScriptModule* module = nullptr;
    };
}