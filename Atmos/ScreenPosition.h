#pragma once

namespace Atmos
{
    struct ScreenPosition
    {
        using Value = int;

        Value x;
        Value y;

        ScreenPosition();
        ScreenPosition(Value x, Value y);
    };
}