#pragma once

#include <Atmos/ScriptAsset.h>
#include <Atmos/ScriptParameters.h>
#include <Atmos/ScriptModule.h>
#include <Atmos/Script.h>

using namespace Atmos;

Arca::Index<Asset::Script> CompileAndCreateScriptAsset(
    const String& name,
    const String& source,
    Arca::Reliquary& reliquary);
static Arca::Index<Asset::Script> CompileAndCreateScriptAsset(
    const String& name,
    const String& source,
    const std::vector<Scripting::Module>& sharedModules,
    Arca::Reliquary& reliquary);

Arca::Index<Scripting::Script> CompileAndCreateScript(
    const String& name,
    const String& source,
    Arca::Reliquary& reliquary);
Arca::Index<Scripting::Script> CompileAndCreateScript(
    const String& name,
    const String& source,
    const Scripting::Parameters& parameters,
    Arca::Reliquary& reliquary);

Arca::Index<Scripting::Script> CompileAndCreateScriptWithSharedModules(
    const String& name,
    const String& source,
    const std::vector<Scripting::Module>& sharedModules,
    Arca::Reliquary& reliquary);
Arca::Index<Scripting::Script> CompileAndCreateScriptWithSharedModules(
    const String& name,
    const String& source,
    const std::vector<Scripting::Module>& sharedModules,
    const Scripting::Parameters& parameters,
    Arca::Reliquary& reliquary);