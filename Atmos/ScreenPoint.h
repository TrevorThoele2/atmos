#pragma once

namespace Atmos::Spatial
{
    struct ScreenPoint
    {
        using Value = int;
        Value x;
        Value y;

        ScreenPoint();
        ScreenPoint(Value x, Value y);

        bool operator==(const ScreenPoint& arg) const;
        bool operator!=(const ScreenPoint& arg) const;
    };
}