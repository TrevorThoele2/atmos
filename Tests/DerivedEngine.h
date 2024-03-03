#pragma once

#include <Atmos/Engine.h>

#include "MockAssetResourceManager.h"
#include "MockWindow.h"
#include "MockAudioManager.h"
#include "MockInputManager.h"
#include "MockGraphicsManager.h"
#include "MockTextManager.h"
#include "MockScriptManager.h"
#include <Atmos/RealWorldManager.h>

using namespace Atmos;

class DerivedEngine final : public Engine
{
public:
    MockAssetResourceManager* mockAssetResourceManager;
    MockWindow* mockWindow;
    MockAudioManager* mockAudioManager;
    MockInputManager* mockInputManager;
    MockGraphicsManager* mockGraphicsManager;
    MockTextManager* mockTextManager;
    MockScriptManager* mockScriptManager;
    std::unique_ptr<World::RealManager> worldManager;

    DerivedEngine(Logging::Logger& logger);
    DerivedEngine(DerivedEngine&& arg) noexcept = default;
private:
    [[nodiscard]] InitializationProperties CreateInitializationProperties(Logging::Logger& logger);
};