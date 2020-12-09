#include "MockScriptManager.h"

void MockScriptManager::Initialize(Arca::Reliquary& reliquary)
{
    
}

void MockScriptManager::RegisterAll()
{
    
}

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

Atmos::File::Path MockScriptManager::Compile(const Atmos::File::Path&, const std::optional<Atmos::File::Path>&)
{
    return {};
}