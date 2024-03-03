#pragma once

#include <Atmos/Engine.h>

#include "MockWindow.h"
#include "MockGraphicsManager.h"

using namespace Atmos;

class DerivedEngine final : public Engine
{
public:
    Arca::Reliquary* globalReliquary = nullptr;
    MockWindow* mockWindow = nullptr;
    MockGraphicsManager* mockGraphicsManager = nullptr;
protected:
    InitializationProperties CreateInitializationProperties(Arca::Reliquary& reliquary) override;

    void DoExit() override;
};