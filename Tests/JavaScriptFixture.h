#pragma once

#include "GeneralFixture.h"
#include <Atmos/Script.h>
#include <Atmos/ScriptAsset.h>
#include <Atmos/String.h>
#include <Atmos/ScriptModule.h>
#include <Atmos/TypeRegistration.h>
#include "JavaScriptEngine.h"
#include <Atmos/Work.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/StringUtility.h>

#include <Arca/RelicIndex.h>
#include <Arca/OpenRelic.h>

class JavaScriptFixture : public GeneralFixture
{
public:
    Atmos::Logging::Logger logger;
    JavaScriptEngine engine;
    Arca::Reliquary* fieldReliquary = nullptr;
    std::vector<Scripting::Finished> finishes;

    JavaScriptFixture();
public:
    Arca::Index<Atmos::Asset::Script> CompileAndCreateScriptAsset(
        const Atmos::String& name,
        const Atmos::String& scriptData,
        Arca::Reliquary& reliquary);
    Arca::Index<Atmos::Asset::Script> CompileAndCreateScriptAsset(
        const Atmos::String& name,
        const Atmos::String& scriptData,
        const std::vector<Atmos::Scripting::Module>& sharedModules,
        Arca::Reliquary& reliquary);

    Arca::Index<Atmos::Scripting::Script> CompileAndCreateScript(
        const Atmos::String& name,
        const Atmos::String& source,
        Arca::Reliquary& reliquary);
    Arca::Index<Atmos::Scripting::Script> CompileAndCreateScript(
        const Atmos::String& name,
        const Atmos::String& source,
        const Atmos::Scripting::Parameters& parameters,
        Arca::Reliquary& reliquary);

    Arca::Index<Atmos::Scripting::Script> CompileAndCreateScriptWithSharedModules(
        const Atmos::String& name,
        const Atmos::String& scriptData,
        const std::vector<Atmos::Scripting::Module>& sharedModules,
        Arca::Reliquary& reliquary);
    Arca::Index<Atmos::Scripting::Script> CompileAndCreateScriptWithSharedModules(
        const Atmos::String& name,
        const Atmos::String& scriptData,
        const std::vector<Atmos::Scripting::Module>& sharedModules,
        const Atmos::Scripting::Parameters& parameters,
        Arca::Reliquary& reliquary);
};