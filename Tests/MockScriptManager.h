#pragma once

#include <Atmos/ScriptManager.h>

class MockScriptManager final : public Atmos::Scripting::Manager
{
public:
    [[nodiscard]] std::unique_ptr<Atmos::Asset::Resource::Script> CreateAssetResource(
        const Atmos::DataBuffer& buffer, Atmos::String name) override;
    [[nodiscard]] std::unique_ptr<Atmos::Scripting::Resource> CreateScriptResource(
        const Atmos::String& scriptAssetName, const Atmos::String& executeName, const Atmos::Scripting::Parameters& parameters) override;
public:
    Atmos::DataBuffer Compile(Atmos::Scripting::Module module, std::vector<Atmos::Scripting::Module> sharedModules) override;
protected:
    void InitializeImpl(Arca::Reliquary& reliquary) override;
};