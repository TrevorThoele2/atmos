#include "MockScriptManager.h"

MockScriptManager::MockScriptManager(Atmos::Logging::Logger& logger) :
    Manager(logger, "Mock")
{}

void MockScriptManager::SetReliquary(Arca::Reliquary& reliquary)
{}

std::unique_ptr<Atmos::Asset::Resource::Script> MockScriptManager::CreateAssetResource(
    const Atmos::Buffer&, Atmos::String)
{
    return {};
}

std::unique_ptr<Atmos::Scripting::Resource> MockScriptManager::CreateScriptResource(
    const Atmos::String& scriptAssetName, const Atmos::String& executeName, const Atmos::Scripting::Parameters& parameters)
{
    return {};
}

Atmos::Buffer MockScriptManager::Compile(Atmos::Scripting::Module module, std::vector<Atmos::Scripting::Module> sharedModules)
{
    return {};
}