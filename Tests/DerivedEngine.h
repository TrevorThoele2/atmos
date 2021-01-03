#pragma once

#include <Atmos/Engine.h>

#include "MockImageAssetManager.h"
#include "MockWindow.h"
#include <Atmos/NullAudioManager.h>
#include "MockInputManager.h"
#include "MockGraphicsManager.h"
#include "MockScriptManager.h"

using namespace Atmos;

class DerivedEngine final : public Engine
{
public:
    MockImageAssetManager* mockImageAssetManager = nullptr;
    MockWindow* mockWindow = nullptr;
    Audio::NullAudioManager* nullAudioManager = nullptr;
    MockInputManager* mockInputManager = nullptr;
    MockGraphicsManager* mockGraphicsManager = nullptr;
    MockScriptManager* mockScriptManager = nullptr;

    DerivedEngine(Logging::Logger& logger);
protected:
    InitializationProperties CreateInitializationProperties(Logging::Logger& logger) override;
};