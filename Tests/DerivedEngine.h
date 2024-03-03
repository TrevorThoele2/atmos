#pragma once

#include <Atmos/Engine.h>

#include "MockWindow.h"
#include "MockGraphicsManager.h"

using namespace Atmos;

class DerivedEngine final : public Engine
{
public:
    MockWindow* mockWindow = nullptr;
    MockGraphicsManager* mockGraphicsManager = nullptr;
public:
    [[nodiscard]] Arca::Reliquary* TheGlobalReliquary();
    [[nodiscard]] const Arca::Reliquary* TheGlobalReliquary() const;
protected:
    void SetupImplementation() override;

    InitializationProperties CreateInitializationProperties() override;

    void DoExit() override;
};