#include "JavaScriptFixture.h"

#include <Atmos/LoadScriptAssetResourceData.h>
#include <Atmos/CompileScript.h>
#include <Arca/Create.h>

#include <Inscription/Plaintext.h>

using namespace Atmos;

JavaScriptFixture::JavaScriptFixture() : logger(Logging::Severity::Verbose), engine(logger)
{
    logger.AddSink<Logging::FileSink>();

    auto fieldOrigin = Arca::ReliquaryOrigin();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockAssetResourceManager,
        *engine.mockAudioManager,
        *engine.mockInputManager,
        *engine.mockGraphicsManager,
        *engine.mockTextManager,
        *engine.scriptManager,
        *engine.mockWorldManager,
        Spatial::Size2D{
            std::numeric_limits<Spatial::Size2D::Value>::max(),
            std::numeric_limits<Spatial::Size2D::Value>::max() },
            *engine.mockWindow,
            engine.Logger());
    fieldOrigin.Register<Arca::OpenRelic>();
    RegisterFieldStages(fieldOrigin);
    engine.mockWorldManager->field = std::make_unique<World::Field>(0, fieldOrigin.Actualize());

    fieldReliquary = &engine.mockWorldManager->field->Reliquary();

    fieldReliquary->On<Scripting::Finished>([this](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });
}

Arca::Index<Atmos::Asset::Script> JavaScriptFixture::CompileAndCreateScriptAsset(
    const Atmos::String& name,
    const Atmos::String& source,
    Arca::Reliquary& reliquary)
{
    return CompileAndCreateScriptAsset(name, source, {}, reliquary);
}

Arca::Index<Atmos::Asset::Script> JavaScriptFixture::CompileAndCreateScriptAsset(
    const Atmos::String& name,
    const Atmos::String& source,
    const std::vector<Atmos::Scripting::Module>& sharedModules,
    Arca::Reliquary& reliquary)
{
    Inscription::Plaintext::ToFile(source, name);
    for (auto& module : sharedModules)
        Inscription::Plaintext::ToFile(module.source, module.name);

    std::vector<Atmos::Scripting::Module> modules = sharedModules;
    modules.push_back(Scripting::Module{ name, source });
    const auto compiledModules = reliquary.Do(Scripting::Compile{ modules });

    std::vector<Atmos::Scripting::CompiledModule>::const_iterator compiledSource;
    for (auto module = compiledModules.begin(); module != compiledModules.end(); ++module)
    {
        if (module->name == File::Path(name).replace_extension().string())
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

Arca::Index<Scripting::Script> JavaScriptFixture::CompileAndCreateScript(
    const String& name, const String& source, Arca::Reliquary& reliquary)
{
    return CompileAndCreateScript(name, source, {}, reliquary);
}

Arca::Index<Scripting::Script> JavaScriptFixture::CompileAndCreateScript(
    const Atmos::String& name,
    const Atmos::String& source,
    const Atmos::Scripting::Parameters& parameters,
    Arca::Reliquary& reliquary)
{
    const auto asset = CompileAndCreateScriptAsset(name, source, reliquary);
    return reliquary.Do(Arca::Create<Scripting::Script>{ asset, "main", parameters });
}

Arca::Index<Atmos::Scripting::Script> JavaScriptFixture::CompileAndCreateScriptWithSharedModules(
    const Atmos::String& name,
    const Atmos::String& source,
    const std::vector<Atmos::Scripting::Module>& sharedModules,
    Arca::Reliquary& reliquary)
{
    return CompileAndCreateScriptWithSharedModules(name, source, sharedModules, Scripting::Parameters{}, reliquary);
}

Arca::Index<Scripting::Script> JavaScriptFixture::CompileAndCreateScriptWithSharedModules(
    const Atmos::String& name,
    const Atmos::String& source,
    const std::vector<Atmos::Scripting::Module>& sharedModules,
    const Atmos::Scripting::Parameters& parameters,
    Arca::Reliquary& reliquary)
{
    for (auto& sharedModule : sharedModules)
        CompileAndCreateScriptAsset(sharedModule.name, sharedModule.source, reliquary);

    const auto asset = CompileAndCreateScriptAsset(name, source, sharedModules, reliquary);
    return reliquary.Do(Arca::Create<Scripting::Script>{ asset, "main", parameters });
}