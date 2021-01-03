#include <Atmos/AngelScriptRegistration.h>
#include <Atmos/AngelScriptManager.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Logger.h>
#include <Atmos/FreeImageAssetManager.h>
#include <Arca/ReliquaryOrigin.h>

#include <angelscript.h>

#include "docgen.h"
#include "DocumentationManager.h"

int main()
{
    Atmos::Logging::Logger logger(Atmos::Logging::Severity::Verbose);
    auto imageAssetManager = std::make_unique<Atmos::Asset::FreeImageManager>(logger);

    Arca::ReliquaryOrigin origin;
    Atmos::RegisterFieldTypes(origin, *imageAssetManager, logger);
    auto reliquary = origin.Actualize();

    auto manager = std::make_unique<Atmos::Scripting::Angel::Manager>(logger);
    manager->Initialize(*reliquary);

    const auto engine = asCreateScriptEngine();
    auto userData = Atmos::Scripting::Angel::UserData();
    userData.manager = manager.get();
    userData.reliquary = reliquary.get();
    engine->SetUserData(&userData);

    auto documentationOptions = ScriptDocumentationOptions();
    documentationOptions.projectName = "An Extraordinary Will";
    documentationOptions.includeArrayInterface = true;
    documentationOptions.includeStringInterface = true;
    documentationOptions.includeWeakRefInterface = false;
    auto documentationGenerator = DocumentationGenerator(engine, documentationOptions);

    DocumentationManager documentationManager(documentationGenerator);
    Atmos::Scripting::Angel::RegisterAll(*engine, documentationManager);
    documentationGenerator.Generate();

    return 0;
}
