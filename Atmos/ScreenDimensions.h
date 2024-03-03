#pragma once

namespace Atmos
{
    class ScreenDimensions
    {
    public:
        typedef unsigned int Dimension;
    public:
        Dimension width;
        Dimension height;

        ScreenDimensions();
        ScreenDimensions(Dimension width, Dimension height);
        ScreenDimensions(const ScreenDimensions& arg);

        bool operator==(const ScreenDimensions& arg) const;
        bool operator!=(const ScreenDimensions& arg) const;
    };
}