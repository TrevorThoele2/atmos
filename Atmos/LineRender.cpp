
#include "LineRender.h"

namespace Atmos
{
    LineRender::LineRender(const Position2D &from, const Position2D &to, const Color &color, Width width) : from(from), to(to), color(color), width(width)
    {}

    void LineRender::SetFrom(const Position2D &set)
    {
        from = set;
    }

    void LineRender::SetTo(const Position2D &set)
    {
        to = set;
    }

    void LineRender::SetWidth(Width set)
    {
        width = set;
    }

    const Position2D& LineRender::GetFrom() const
    {
        return from;
    }

    const Position2D& LineRender::GetTo() const
    {
        return to;
    }

    LineRender::Width LineRender::GetWidth() const
    {
        return width;
    }

    void LineRender::Set(const Position2D &from, const Position2D &to)
    {
        this->from = from;
        this->to = to;
    }
}