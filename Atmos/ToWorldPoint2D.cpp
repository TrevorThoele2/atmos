#include "ToWorldPoint2D.h"

#include "Camera.h"

namespace Atmos::Spatial
{
    ToWorldPoint2D::ToWorldPoint2D(Point2D from) : from(from)
    {}

    Point2D ToWorldPoint2D::Do(Arca::Reliquary& reliquary) const
    {
        const auto camera = Arca::Index<Render::Camera>(reliquary);
        const auto position = camera->Position();
        const auto size = camera->Size();
        return { from.x + position.x - size.width / 2, from.y + position.y - size.height / 2 };
    }
}