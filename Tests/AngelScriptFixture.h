#pragma once

#include "GeneralFixture.h"
#include <Atmos/Script.h>
#include <Atmos/ScriptAsset.h>
#include <Atmos/String.h>
#include <Atmos/ScriptModule.h>

#include <Arca/RelicIndex.h>

class AngelScriptFixture : public GeneralFixture
{
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
        const Atmos::String& scriptData,
        Arca::Reliquary& reliquary);
    Arca::Index<Atmos::Scripting::Script> CompileAndCreateScript(
        const Atmos::String& name,
        const Atmos::String& scriptData,
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