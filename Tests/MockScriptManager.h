#pragma once

#include <Atmos/ScriptManager.h>

class MockScriptManager final : public Atmos::Scripting::Manager
{
public:
    void SetReliquary(Arca::Reliquary& reliquary) override;

    [[nodiscard]] std::unique_ptr<Atmos::Asset::Resource::Script> CreateAssetResource(
        const Atmos::Buffer& buffer, Atmos::String name) override;
    [[nodiscard]] std::unique_ptr<Atmos::Scripting::Resource> CreateScriptResource(
        const Atmos::String& scriptAssetName, const Atmos::String& executeName, const Atmos::Scripting::Parameters& parameters) override;

    Atmos::Buffer Compile(Atmos::Scripting::Module module, std::vector<Atmos::Scripting::Module> sharedModules) override;

    [[nodiscard]] Atmos::String TypeName() const override;
};