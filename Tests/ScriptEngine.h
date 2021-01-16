#pragma once

#include <Atmos/Engine.h>

#include "MockAssetResourceManager.h"
#include "MockWindow.h"
#include "MockAudioManager.h"
#include "MockInputManager.h"
#include "MockGraphicsManager.h"
#include <Atmos/AngelScriptManager.h>
#include "MockWorldManager.h"

using namespace Atmos;

class ScriptEngine final : public Engine
{
public:
    MockAssetResourceManager* mockImageAssetManager;
    MockWindow* mockWindow;
    MockAudioManager* mockAudioManager;
    MockInputManager* mockInputManager;
    MockGraphicsManager* mockGraphicsManager;
    Scripting::Angel::Manager* scriptManager;
    MockWorldManager* mockWorldManager;

    ScriptEngine(Logging::Logger& logger);
private:
    [[nodiscard]] InitializationProperties CreateInitializationProperties(Logging::Logger& logger);
};