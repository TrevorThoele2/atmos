#pragma once

#include <Atmos/ScriptManager.h>

class MockScriptManager final : public Atmos::Scripting::Manager
{
public:
    MockScriptManager(Atmos::Logging::Logger& logger);

    void SetReliquary(Arca::Reliquary* reliquary) override;

    [[nodiscard]] std::unique_ptr<Atmos::Asset::Resource::Script> CreateAssetResource(
        const Atmos::Buffer& buffer, Atmos::String name) override;
    [[nodiscard]] std::unique_ptr<Atmos::Scripting::Resource> CreateScriptResource(
        const Atmos::Asset::Resource::Script& asset,
        const Atmos::String& assetName,
        const Atmos::String& executeName,
        const Atmos::Scripting::Parameters& parameters) override;

    std::vector<Atmos::Scripting::CompiledModule> Compile(
        const std::vector<Atmos::Scripting::Module>& modules) override;
};