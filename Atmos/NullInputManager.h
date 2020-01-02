#pragma once

#include "InputManager.h"

namespace Atmos::Input
{
    class NullManager final : public Manager
    {
    private:
        [[nodiscard]] std::vector<CreatedInput> CreateAllInputs() const override;
        [[nodiscard]] ScreenPosition CurrentMousePosition() const override;
        [[nodiscard]] bool IsInputDown(Input& input) const override;
    };
}