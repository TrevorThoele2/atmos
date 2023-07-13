#include "GridRegion.h"

namespace Atmos::Render
{
    GridRegion::GridRegion(Arca::RelicInit init) :
        renderCore(init.Create<RenderCore>())
    {}

    GridRegion::GridRegion(
        Arca::RelicInit init,
        const std::unordered_set<Spatial::Grid::Point>& points,
        Spatial::Grid::Point::Value z,
        const Arca::Index<Asset::Material>& material)
        :
        renderCore(init.Create<RenderCore>(material, Color{})), points(points), z(z)
    {}

    bool GridRegion::operator==(const GridRegion& arg) const
    {
        return IndexEqualsValue(renderCore, arg.renderCore) && points == arg.points && z == arg.z;
    }

    bool GridRegion::operator!=(const GridRegion& arg) const
    {
        return !(*this == arg);
    }
}