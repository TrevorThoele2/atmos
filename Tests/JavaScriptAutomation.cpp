#include "JavaScriptAutomation.h"

#include <Arca/Create.h>
#include <Atmos/ScriptCompiledModule.h>
#include <Atmos/CompileScript.h>
#include <Inscription/Plaintext.h>

Arca::Index<Asset::Script> CompileAndCreateScriptAsset(
    const String& name,
    const String& source,
    Arca::Reliquary& reliquary)
{
    return CompileAndCreateScriptAsset(name, source, {}, reliquary);
}

Arca::Index<Asset::Script> CompileAndCreateScriptAsset(
    const String& name,
    const String& source,
    const std::vector<Scripting::Module>& sharedModules,
    Arca::Reliquary& reliquary)
{
    Inscription::Plaintext::ToFile(source, name);
    for (auto& module : sharedModules)
        Inscription::Plaintext::ToFile(module.source, module.name);

    std::vector<Scripting::Module> modules = sharedModules;
    modules.push_back(Scripting::Module{ name, source });
    const auto compiledModules = reliquary.Do(Scripting::Compile{ modules });

    std::vector<Scripting::CompiledModule>::const_iterator compiledSource;
    for (auto module = compiledModules.begin(); module != compiledModules.end(); ++module)
    {
        if (module->name == Inscription::File::Path(name).replace_extension().string())
        {
            compiledSource = module;
            break;
        }
    }

    auto assetResource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Script>{
        compiledSource->source,
        name });

    return reliquary.Do(Arca::Create<Asset::Script>{
        name, std::move(assetResource)});
}

Arca::Index<Scripting::Script> CompileAndCreateScript(
    const String& name, const String& source, Arca::Reliquary& reliquary)
{
    return CompileAndCreateScript(name, source, {}, reliquary);
}

Arca::Index<Scripting::Script> CompileAndCreateScript(
    const String& name,
    const String& source,
    const Scripting::Parameters& parameters,
    Arca::Reliquary& reliquary)
{
    const auto asset = CompileAndCreateScriptAsset(name, source, reliquary);
    return reliquary.Do(Arca::Create<Scripting::Script>{ asset, "main", parameters });
}

Arca::Index<Scripting::Script> CompileAndCreateScriptWithSharedModules(
    const String& name,
    const String& source,
    const std::vector<Scripting::Module>& sharedModules,
    Arca::Reliquary& reliquary)
{
    return CompileAndCreateScriptWithSharedModules(name, source, sharedModules, Scripting::Parameters{}, reliquary);
}

Arca::Index<Scripting::Script> CompileAndCreateScriptWithSharedModules(
    const String& name,
    const String& source,
    const std::vector<Scripting::Module>& sharedModules,
    const Scripting::Parameters& parameters,
    Arca::Reliquary& reliquary)
{
    for (auto& sharedModule : sharedModules)
        CompileAndCreateScriptAsset(sharedModule.name, sharedModule.source, reliquary);

    const auto asset = CompileAndCreateScriptAsset(name, source, sharedModules, reliquary);
    return reliquary.Do(Arca::Create<Scripting::Script>{ asset, "main", parameters });
}