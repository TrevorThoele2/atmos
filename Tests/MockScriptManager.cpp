#include "MockScriptManager.h"

std::unique_ptr<Atmos::Asset::Resource::Script> MockScriptManager::CreateAssetResource(
    const Atmos::DataBuffer&, Atmos::String)
{
    return {};
}

std::unique_ptr<Atmos::Scripting::Resource> MockScriptManager::CreateScriptResource(
    const Atmos::String& scriptAssetName, const Atmos::String& executeName, const Atmos::Scripting::Parameters& parameters)
{
    return {};
}

Atmos::DataBuffer MockScriptManager::Compile(Atmos::Scripting::Module module, std::vector<Atmos::Scripting::Module> sharedModules)
{
    return {};
}

void MockScriptManager::InitializeImpl(Arca::Reliquary& reliquary)
{

}