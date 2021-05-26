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
        Arca::Index<Asset::Material> material)
        :
        renderCore(init.Create<RenderCore>(material, Color{})), points(points), z(z)
    {}
}