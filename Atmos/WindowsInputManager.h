#pragma once

#include "InputManager.h"

#include "WindowsInclude.h"

namespace Atmos::Input
{
    class WindowsManager final : public Manager
    {
    public:
        explicit WindowsManager(HWND hwnd);

        [[nodiscard]] std::vector<CreatedInput> CreateAllInputs() const override;
        [[nodiscard]] Spatial::ScreenPoint CurrentMousePosition() const override;
        [[nodiscard]] bool IsInputDown(Input& input) const override;
    private:
        HWND hwnd;
    };
}