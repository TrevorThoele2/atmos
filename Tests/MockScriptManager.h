#pragma once

#include <Atmos/ScriptManager.h>

class MockScriptManager final : public Atmos::Scripting::Manager
{
public:
    void Initialize(Arca::Reliquary& reliquary) override;

    void RegisterAll() override;

    [[nodiscard]] std::unique_ptr<Atmos::Asset::Resource::Script> CreateAssetResource(
        const Atmos::DataBuffer& buffer, Atmos::String name) override;
    [[nodiscard]] std::unique_ptr<Atmos::Scripting::Resource> CreateScriptResource(
        const Atmos::String& scriptAssetName, const Atmos::String& executeName, const Atmos::Scripting::Parameters& parameters) override;
public:
    Atmos::File::Path Compile(const Atmos::File::Path& inputFilePath,
        const std::optional<Atmos::File::Path>& outputFilePath) override;
};