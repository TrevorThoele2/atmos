#pragma once

namespace Atmos
{
    struct ScreenSize
    {
        using Dimension = int;
        Dimension width = 0;
        Dimension height = 0;

        ScreenSize() = default;
        ScreenSize(Dimension width, Dimension height);
        ScreenSize(const ScreenSize& arg) = default;

        bool operator==(const ScreenSize& arg) const;
        bool operator!=(const ScreenSize& arg) const;
    };
}