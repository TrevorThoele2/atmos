#include "ToWorldPoint2D.h"

#include "Camera.h"

namespace Atmos::Spatial
{
    ToWorldPoint2D::ToWorldPoint2D(ScreenPoint from) : from(from)
    {}

    Point2D ToWorldPoint2D::Do(Arca::Reliquary& reliquary) const
    {
        const auto camera = Arca::Index<Render::Camera>(reliquary);
        const auto position = camera->Position();
        return { from.x + position.x, from.y + position.y };
    }
}