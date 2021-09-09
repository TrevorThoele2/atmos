#pragma once

#include "ScriptManager.h"

namespace Atmos::Scripting
{
    class NullManager final : public Manager
    {
    public:
        NullManager(Logging::Logger& logger);

        void SetReliquary(Arca::Reliquary* reliquary) override;

        [[nodiscard]] std::unique_ptr<Asset::Resource::Script> CreateAssetResource(
            const Buffer& buffer, String name) override;
        [[nodiscard]] std::unique_ptr<Resource> CreateScriptResource(
            const String& scriptAssetName, const String& executeName, const Parameters& parameters) override;

        Buffer Compile(Module module, std::vector<Module> sharedModules) override;
    };
}