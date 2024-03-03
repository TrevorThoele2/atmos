#pragma once

#include <Atmos/Engine.h>

#include "MockWindow.h"
#include <Atmos/NullAudioManager.h>
#include <Atmos/NullInputManager.h>
#include "MockGraphicsManager.h"

using namespace Atmos;

class DerivedEngine final : public Engine
{
public:
    MockWindow* mockWindow = nullptr;
    Audio::NullAudioManager* nullAudioManager = nullptr;
    Input::NullManager* nullInputManager = nullptr;
    MockGraphicsManager* mockGraphicsManager = nullptr;
protected:
    void SetupImplementation() override;

    InitializationProperties CreateInitializationProperties() override;

    void DoExit() override;
};