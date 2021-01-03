#pragma once

#include <Atmos/Engine.h>
#include "MockImageAssetManager.h"
#include "MockWindow.h"
#include <Atmos/NullAudioManager.h>
#include "MockInputManager.h"
#include "MockGraphicsManager.h"
#include <Atmos/AngelScriptManager.h>

using namespace Atmos;

class ScriptEngine final : public Engine
{
public:
    MockImageAssetManager* mockImageAssetManager = nullptr;
    MockWindow* mockWindow = nullptr;
    Audio::NullAudioManager* nullAudioManager = nullptr;
    MockInputManager* mockInputManager = nullptr;
    MockGraphicsManager* mockGraphicsManager = nullptr;
    Scripting::Angel::Manager* scriptManager = nullptr;

    ScriptEngine(Logging::Logger& logger);
protected:
    InitializationProperties CreateInitializationProperties(Logging::Logger& logger) override;
};