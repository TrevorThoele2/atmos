#include <Atmos/AngelScriptRegistration.h>
#include <Atmos/AngelScriptManager.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Logger.h>
#include <Atmos/FileLoggingSink.h>
#include <Atmos/RealAssetResourceManager.h>
#include <Arca/ReliquaryOrigin.h>

#include <angelscript.h>

#include "docgen.h"
#include "DocumentationManager.h"

int main()
{
    Atmos::Logging::Logger logger(Atmos::Logging::Severity::Verbose);
    logger.Add<Atmos::Logging::FileSink>();
    auto assetResourceManager = std::make_unique<Atmos::Asset::Resource::RealManager>(logger);

    Arca::ReliquaryOrigin origin;
    Atmos::RegisterFieldTypes(origin, *assetResourceManager, logger);
    auto reliquary = origin.Actualize();

    auto manager = std::make_unique<Atmos::Scripting::Angel::Manager>(logger);
    manager->SetReliquary(*reliquary);

    const auto engine = asCreateScriptEngine();
    auto userData = Atmos::Scripting::Angel::UserData();
    userData.reliquary = reliquary.get();
    engine->SetUserData(&userData);

    auto documentationOptions = ScriptDocumentationOptions();
    documentationOptions.projectName = "An Extraordinary Will";
    documentationOptions.includeArrayInterface = true;
    documentationOptions.includeStringInterface = true;
    documentationOptions.includeWeakRefInterface = false;
    documentationOptions.syntaxHighlight = true;
    auto documentationGenerator = DocumentationGenerator(engine, documentationOptions);

    DocumentationManager documentationManager(documentationGenerator);
    Atmos::Scripting::Angel::RegisterAll(*engine, documentationManager);
    documentationGenerator.Generate();

    return 0;
}
