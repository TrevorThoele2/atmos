#pragma once

#include "Position2D.h"

namespace Atmos
{
    class MousePosition
    {
    public:
        typedef int Value;
    public:
        Value x, y;
    public:
        MousePosition(Value x = 0, Value y = 0);
        MousePosition(const MousePosition &arg) = default;

        MousePosition& operator=(const MousePosition &arg) = default;

        bool operator==(const MousePosition &arg) const;
        bool operator!=(const MousePosition &arg) const;

        void Set(Value x, Value y);

        operator Position2D() const;
        Position2D ToPosition() const;
    };

    class MousePositionDelta
    {
    public:
        MousePositionDelta();
        MousePositionDelta(const MousePosition& mousePos, const MousePosition& mousePosPrev);

        // Sets current and previous to mousePos
        void Setup(const MousePosition& mousePos);
        void Update(const MousePosition& mousePos);

        bool HasMoved() const;
        const MousePosition& Current() const;
        const MousePosition& Previous() const;
        const MousePosition& Delta() const;
    private:
        MousePosition current, previous, delta;
    private:
        void CalculateDelta();
    };
}