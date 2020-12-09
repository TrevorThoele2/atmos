#pragma once

#include <Atmos/Engine.h>
#include "MockImageAssetManager.h"
#include "MockWindow.h"
#include <Atmos/NullAudioManager.h>
#include <Atmos/NullInputManager.h>
#include "MockGraphicsManager.h"
#include <Atmos/AngelScriptManager.h>

using namespace Atmos;

class ScriptEngine final : public Engine
{
public:
    MockImageAssetManager* mockImageAssetManager = nullptr;
    MockWindow* mockWindow = nullptr;
    Audio::NullAudioManager* nullAudioManager = nullptr;
    Input::NullManager* nullInputManager = nullptr;
    MockGraphicsManager* mockGraphicsManager = nullptr;
    Scripting::Angel::Manager* scriptManager = nullptr;
protected:
    void SetupImplementation() override;

    InitializationProperties CreateInitializationProperties(Logging::Logger& logger) override;

    void DoExit() override;
};