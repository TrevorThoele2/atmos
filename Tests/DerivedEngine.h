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
    void SetupImplementation() override;

    InitializationProperties CreateInitializationProperties(Arca::Reliquary& reliquary) override;

    void DoExit() override;
};