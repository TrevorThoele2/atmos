#include "Line.h"

namespace Atmos::Render
{
    Line::Line(const std::vector<Spatial::Point2D>& points, Arca::Index<Asset::LineMaterial> material) :
        points(points), material(material)
    {}

    Line::Line(
        const std::vector<Spatial::Point2D>& points,
        Spatial::Point2D::Value z,
        Arca::Index<Asset::LineMaterial> material,
        LineWidth width,
        Color color)
        :
        points(points), z(z), material(material), width(width), color(color)
    {}
}