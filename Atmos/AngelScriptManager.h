#pragma once

#include "ScriptManager.h"
#include <angelscript.h>

namespace Atmos::Script::Angel
{
    class Manager final : public Script::Manager
    {
    public:
        [[nodiscard]] std::unique_ptr<Asset::ScriptAssetData> CreateAssetData(String name, String fileData) override;
    private:
        asIScriptEngine* engine;
    };
}
