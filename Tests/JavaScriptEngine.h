#pragma once

#include <Atmos/Engine.h>

#include "MockAssetResourceManager.h"
#include "MockWindow.h"
#include "MockAudioManager.h"
#include "MockInputManager.h"
#include "MockGraphicsManager.h"
#include "MockTextManager.h"
#include <Atmos/JavaScriptManager.h>
#include "MockWorldManager.h"

using namespace Atmos;

class JavaScriptEngine final : public Engine
{
public:
    MockAssetResourceManager* mockAssetResourceManager;
    MockWindow* mockWindow;
    MockAudioManager* mockAudioManager;
    MockInputManager* mockInputManager;
    MockGraphicsManager* mockGraphicsManager;
    MockTextManager* mockTextManager;
    Scripting::JavaScript::Manager* scriptManager;
    MockWorldManager* mockWorldManager;

    explicit JavaScriptEngine(Logging::Logger& logger);
    JavaScriptEngine(JavaScriptEngine&& arg) noexcept = default;
private:
    [[nodiscard]] InitializationProperties CreateInitializationProperties(Logging::Logger& logger);
};