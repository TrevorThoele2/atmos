#include "Line.h"

namespace Atmos::Render
{
    Line::Line(Arca::RelicInit init) :
        renderCore(init.Create<RenderCore>())
    {}

    Line::Line(Arca::RelicInit init, const std::vector<Spatial::Point2D>& points, const Material& material) :
        renderCore(init.Create<RenderCore>(material, Color{})), points(points)
    {}

    Line::Line(
        Arca::RelicInit init,
        const std::vector<Spatial::Point2D>& points,
        Spatial::Point2D::Value z,
        const Material& material,
        LineWidth width,
        Color color)
        :
        renderCore(init.Create<RenderCore>(material, color)), points(points), z(z), width(width)
    {}

    Line::Line(Arca::RelicInit init, Arca::Serialization) :
        renderCore(init.Find<RenderCore>())
    {}

    bool Line::operator==(const Line& arg) const
    {
        return IndexEqualsValue(renderCore, arg.renderCore) && points == arg.points && z == arg.z && width == arg.width;
    }

    bool Line::operator!=(const Line& arg) const
    {
        return !(*this == arg);
    }
}