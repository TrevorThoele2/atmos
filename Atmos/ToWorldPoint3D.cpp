#include "ToWorldPoint3D.h"

#include "Camera.h"

namespace Atmos::Spatial
{
    ToWorldPoint3D::ToWorldPoint3D(ScreenPoint from, Point3D::Value z) : from(from), z(z)
    {}

    Point3D ToWorldPoint3D::Do(Arca::Reliquary& reliquary) const
    {
        const auto camera = Arca::Index<Render::Camera>(reliquary);
        const auto position = camera->Position();
        return { from.x + position.x, from.y + position.y };
    }
}