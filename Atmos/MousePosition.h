#pragma once

#include "Position2D.h"

namespace Atmos
{
    class MousePosition
    {
    public:
        typedef int ValueT;
    public:
        ValueT x, y;
        MousePosition(ValueT x = 0, ValueT y = 0);
        MousePosition(const MousePosition &arg) = default;
        MousePosition& operator=(const MousePosition &arg) = default;
        bool operator==(const MousePosition &arg) const;
        bool operator!=(const MousePosition &arg) const;

        void Set(ValueT x, ValueT y);

        operator Position2D() const;
        Position2D ToPosition() const;
    };

    class MousePositionDelta
    {
    private:
        MousePosition now, prev, delta;
        void CalcDelta();
    public:
        MousePositionDelta();
        MousePositionDelta(const MousePosition &mousePos, const MousePosition &mousePosPrev);
        // Sets now and prev to mousePos
        void Setup(const MousePosition &mousePos);
        void Update(const MousePosition &mousePos);

        bool Moved() const;
        const MousePosition& GetNow() const;
        const MousePosition& GetPrevious() const;
        const MousePosition& GetDelta() const;
    };
}