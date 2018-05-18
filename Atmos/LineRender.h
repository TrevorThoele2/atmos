#pragma once

#include "Position2D.h"
#include "Color.h"

namespace Atmos
{
    class LineRender
    {
    public:
        typedef float Width;
    private:
        Position2D from;
        Position2D to;
        Width width;
    public:
        Color color;
        LineRender(const Position2D &from, const Position2D &to, const Color &color, Width = 1.0f);
        void SetFrom(const Position2D &set);
        void SetTo(const Position2D &set);
        void SetWidth(Width set);
        const Position2D& GetFrom() const;
        const Position2D& GetTo() const;
        Width GetWidth() const;
        void Set(const Position2D &from, const Position2D &to);
    };
}