#include "AngelScriptFixture.h"

#include <Atmos/LoadScriptAssetResourceData.h>
#include <Atmos/CompileScript.h>
#include <Inscription/OutputTextArchive.h>

using namespace Atmos;

Arca::Index<Atmos::Asset::Script> AngelScriptFixture::CompileAndCreateScriptAsset(
    const Atmos::String& name,
    const Atmos::String& scriptData,
    Arca::Reliquary& reliquary)
{
    return CompileAndCreateScriptAsset(name, scriptData, {}, reliquary);
}

Arca::Index<Atmos::Asset::Script> AngelScriptFixture::CompileAndCreateScriptAsset(
    const Atmos::String& name,
    const Atmos::String& scriptData,
    const std::vector<Atmos::Scripting::Module>& sharedModules,
    Arca::Reliquary& reliquary)
{
    const auto compiledData = reliquary.Do(Scripting::Compile{ Scripting::Module{name, scriptData}, sharedModules });

    auto basicScriptResource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Script>{
        compiledData,
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
    return reliquary.Do(Arca::Create<Scripting::Script>{ asset, "main", parameters });
}

Arca::Index<Atmos::Scripting::Script> AngelScriptFixture::CompileAndCreateScriptWithSharedModules(
    const Atmos::String& name,
    const Atmos::String& scriptData,
    const std::vector<Atmos::Scripting::Module>& sharedModules,
    Arca::Reliquary& reliquary)
{
    const auto asset = CompileAndCreateScriptAsset(name, scriptData, sharedModules, reliquary);
    return reliquary.Do(Arca::Create<Scripting::Script>{ asset, "main", Scripting::Parameters{} });
}

Arca::Index<Scripting::Script> AngelScriptFixture::CompileAndCreateScriptWithSharedModules(
    const Atmos::String& name,
    const Atmos::String& scriptData,
    const std::vector<Atmos::Scripting::Module>& sharedModules,
    const Atmos::Scripting::Parameters& parameters,
    Arca::Reliquary& reliquary)
{
    const auto asset = CompileAndCreateScriptAsset(name, scriptData, sharedModules, reliquary);
    return reliquary.Do(Arca::Create<Scripting::Script>{ asset, "main", parameters });
}