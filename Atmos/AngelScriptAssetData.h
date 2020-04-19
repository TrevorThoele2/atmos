#pragma once

#include "ScriptAssetData.h"
#include <angelscript.h>

namespace Atmos::Script::Angel
{
    class AssetDataImplementation final : public Asset::ScriptAssetData
    {
    public:
        explicit AssetDataImplementation(asIScriptModule& module);

        [[nodiscard]] asIScriptModule* Module() const;
    private:
        asIScriptModule* module = nullptr;
    };
}