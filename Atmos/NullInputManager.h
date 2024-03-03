#pragma once

#include "InputManager.h"

namespace Atmos::Input
{
    class NullManager final : public Manager
    {
    public:
        [[nodiscard]] State ReadState() const override;
    };
}