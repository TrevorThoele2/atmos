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
            const Asset::Resource::Script& asset,
            const String& assetName,
            const String& executeName,
            const Parameters& parameters) override;

        std::vector<CompiledModule> Compile(const std::vector<Module>& modules) override;
    };
}