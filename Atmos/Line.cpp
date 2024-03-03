#include "Line.h"

namespace Atmos::Render
{
    Line::Line(const Position2D& from, const Position2D& to, const Color& color, Width width) :
        from(from), to(to), color(color), width(width)
    {}
}