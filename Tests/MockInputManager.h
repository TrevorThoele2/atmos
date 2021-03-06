#pragma once

#include <Atmos/InputManager.h>

class MockInputManager final : public Atmos::Input::Manager
{
public:
    State sendState;
public:
    MockInputManager(Atmos::Logging::Logger& logger);

    [[nodiscard]] State ReadState() const override;
};