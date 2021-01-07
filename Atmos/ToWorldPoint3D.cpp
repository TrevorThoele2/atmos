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
        const auto size = camera->Size();
        return { from.x + position.x - size.width / 2, from.y + position.y - size.height / 2, z };
    }
}