#include "MockScriptManager.h"

MockScriptManager::MockScriptManager(Atmos::Logging::Logger& logger) :
    Manager(logger, "Mock")
{}

void MockScriptManager::SetReliquary(Arca::Reliquary* reliquary)
{}

std::unique_ptr<Atmos::Asset::Resource::Script> MockScriptManager::CreateAssetResource(
    const Atmos::Buffer&, Atmos::String)
{
    return {};
}

std::unique_ptr<Atmos::Scripting::Resource> MockScriptManager::CreateScriptResource(
    const Atmos::Asset::Resource::Script& asset,
    const Atmos::String& assetName,
    const Atmos::String& executeName,
    const Atmos::Scripting::Parameters& parameters)
{
    return {};
}

std::vector<Atmos::Scripting::CompiledModule> MockScriptManager::Compile(
    const std::vector<Atmos::Scripting::Module>& modules)
{
    return {};
}