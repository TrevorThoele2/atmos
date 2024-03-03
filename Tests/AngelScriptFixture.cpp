#include "AngelScriptFixture.h"

#include <Atmos/LoadScriptAssetResourceData.h>
#include <Atmos/CompileScript.h>
#include "Atmos/CreateScriptResource.h"
#include <Inscription/OutputTextArchive.h>

using namespace Atmos;

Arca::Index<Atmos::Asset::Script> AngelScriptFixture::CompileAndCreateScriptAsset(
    const Atmos::String& name,
    const Atmos::String& scriptData,
    Arca::Reliquary& reliquary)
{
    const auto basicScriptFilePath = std::filesystem::current_path() / name;

    {
        auto outputArchive = Inscription::OutputTextArchive(basicScriptFilePath);
        outputArchive.Write(scriptData);
    }

    auto compiledScriptFilePath = reliquary.Do(Scripting::Compile{ basicScriptFilePath });

    auto basicScriptResourceData = reliquary.Do(
        Asset::Resource::LoadDataFromFile<Asset::Resource::Script>{ compiledScriptFilePath });

    auto basicScriptResource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Script>{
        basicScriptResourceData.data,
            name });

    return reliquary.Do(Arca::Create<Asset::Script>{
        name, std::move(basicScriptResource)});
}

Arca::Index<Scripting::Script> AngelScriptFixture::CompileAndCreateScript(
    const String& name, const String& scriptData, Arca::Reliquary& reliquary)
{
    return CompileAndCreateScript(name, scriptData, {}, reliquary);
}

Arca::Index<Scripting::Script> AngelScriptFixture::CompileAndCreateScript(
    const Atmos::String& name,
    const Atmos::String& scriptData,
    const Atmos::Scripting::Parameters& parameters,
    Arca::Reliquary& reliquary)
{
    const auto asset = CompileAndCreateScriptAsset(name, scriptData, reliquary);
    auto resource = reliquary.Do(Scripting::CreateResource{ asset, "main", parameters });
    return reliquary.Do(Arca::Create<Scripting::Script>{ std::move(resource) });
}