#include "Line.h"

namespace Atmos::Render
{
    Line::Line(Init init, const std::vector<Spatial::Point2D>& points, Arca::Index<Asset::Material> material) :
        ClosedTypedRelic(init), points(points), material(material)
    {}

    Line::Line(
        Init init,
        const std::vector<Spatial::Point2D>& points,
        Spatial::Point2D::Value z,
        Arca::Index<Asset::Material> material,
        LineWidth width,
        Color color)
        :
        ClosedTypedRelic(init),
        points(points), z(z), material(material), width(width), color(color)
    {}
}