#pragma once

#include <Atmos/InputManager.h>

class MockInputManager final : public Atmos::Input::Manager
{
public:
    State sendState;
public:
    MockInputManager() = default;

    [[nodiscard]] State ReadState() const override;
};