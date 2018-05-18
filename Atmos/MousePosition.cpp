
#include "MousePosition.h"

namespace Atmos
{
    MousePosition::MousePosition(ValueT x, ValueT y) : x(x), y(y)
    {}

    bool MousePosition::operator==(const MousePosition &arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool MousePosition::operator!=(const MousePosition &arg) const
    {
        return !(*this == arg);
    }

    void MousePosition::Set(ValueT x, ValueT y)
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
        return Position2D(static_cast<Position2D::ValueT>(x), static_cast<Position2D::ValueT>(y));
    }

    void MousePositionDelta::CalcDelta()
    {
        delta.x = prev.x - now.x;
        delta.y = prev.y - now.y;
    }

    MousePositionDelta::MousePositionDelta()
    {
        now.Set(0, 0);
        prev.Set(0, 0);
        delta.Set(0, 0);
    }

    MousePositionDelta::MousePositionDelta(const MousePosition &mousePos, const MousePosition &mousePosPrev) : now(mousePos), prev(mousePosPrev)
    {
        CalcDelta();
    }

    void MousePositionDelta::Setup(const MousePosition &mousePos)
    {
        now = mousePos;
        prev = mousePos;
        CalcDelta();
    }

    void MousePositionDelta::Update(const MousePosition &mousePos)
    {
        now = mousePos;
        CalcDelta();
    }

    bool MousePositionDelta::Moved() const
    {
        return delta.x != 0 || delta.y != 0;
    }

    const MousePosition& MousePositionDelta::GetNow() const
    {
        return now;
    }

    const MousePosition& MousePositionDelta::GetPrevious() const
    {
        return prev;
    }

    const MousePosition& MousePositionDelta::GetDelta() const
    {
        return delta;
    }
}