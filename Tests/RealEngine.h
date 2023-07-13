#pragma once

#include <Atmos/Engine.h>

#include "MockAssetResourceManager.h"
#include "MockWindow.h"
#include "MockAudioManager.h"
#include "MockInputManager.h"
#include "MockGraphicsManager.h"
#include "MockTextManager.h"
#include <Atmos/JavaScriptManager.h>
#include <Atmos/RealWorldManager.h>

using namespace Atmos;

class RealEngine final : public Engine
{
public:
    MockAssetResourceManager* mockAssetResourceManager;
    MockWindow* mockWindow;
    MockAudioManager* mockAudioManager;
    MockInputManager* mockInputManager;
    MockGraphicsManager* mockGraphicsManager;
    MockTextManager* mockTextManager;
    Scripting::JavaScript::Manager* scriptManager;
    World::RealManager* worldManager;

    explicit RealEngine(Logging::Logger& logger);
    RealEngine(RealEngine&& arg) noexcept = default;
private:
    [[nodiscard]] InitializationProperties CreateInitializationProperties(Logging::Logger& logger);
};