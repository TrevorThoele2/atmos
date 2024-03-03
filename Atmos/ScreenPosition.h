#pragma once

namespace Atmos
{
    class ScreenPosition
    {
    public:
        typedef unsigned int Dimension;
    public:
        Dimension x;
        Dimension y;
    public:
        ScreenPosition();
        ScreenPosition(Dimension x, Dimension y);
    };
}