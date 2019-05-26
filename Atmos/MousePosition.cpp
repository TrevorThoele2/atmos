
#include "MousePosition.h"

namespace Atmos
{
    MousePosition::MousePosition(Value x, Value y) : x(x), y(y)
    {}

    bool MousePosition::operator==(const MousePosition& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool MousePosition::operator!=(const MousePosition& arg) const
    {
        return !(*this == arg);
    }

    void MousePosition::Set(Value x, Value y)
    {
        this->x = x;
        this->y = y;
    }

    MousePosition::operator Position2D() const
    {
        return ToPosition();
    }

    Position2D MousePosition::ToPosition() const
    {
        return Position2D(static_cast<Position2D::Value>(x), static_cast<Position2D::Value>(y));
    }

    void MousePositionDelta::CalculateDelta()
    {
        delta.x = previous.x - current.x;
        delta.y = previous.y - current.y;
    }

    MousePositionDelta::MousePositionDelta()
    {
        current.Set(0, 0);
        previous.Set(0, 0);
        delta.Set(0, 0);
    }

    MousePositionDelta::MousePositionDelta(const MousePosition& mousePos, const MousePosition& mousePosPrev) :
        current(mousePos), previous(mousePosPrev)
    {
        CalculateDelta();
    }

    void MousePositionDelta::Setup(const MousePosition& mousePos)
    {
        current = mousePos;
        previous = mousePos;
        CalculateDelta();
    }

    void MousePositionDelta::Update(const MousePosition& mousePos)
    {
        current = mousePos;
        CalculateDelta();
    }

    bool MousePositionDelta::HasMoved() const
    {
        return delta.x != 0 || delta.y != 0;
    }

    const MousePosition& MousePositionDelta::Current() const
    {
        return current;
    }

    const MousePosition& MousePositionDelta::Previous() const
    {
        return previous;
    }

    const MousePosition& MousePositionDelta::Delta() const
    {
        return delta;
    }
}