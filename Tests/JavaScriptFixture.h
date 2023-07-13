#pragma once

#include "GeneralFixture.h"
#include <Atmos/TypeRegistration.h>
#include "JavaScriptEngine.h"
#include <Atmos/Work.h>
#include <Atmos/ScriptFinished.h>
#include "JavaScriptAutomation.h"
#include <Arca/OpenRelic.h>

class JavaScriptFixture : public GeneralFixture
{
public:
    Logging::Logger logger;
    JavaScriptEngine engine;
    Arca::Reliquary* fieldReliquary = nullptr;
    std::vector<Scripting::Finished> finishes;

    JavaScriptFixture();
};