#include "GridRegion.h"

namespace Atmos::Render
{
    GridRegion::GridRegion(Init init) :
        ClosedTypedRelic(init)
    {}

    GridRegion::GridRegion(
        Init init,
        const std::unordered_set<Spatial::Grid::Point>& points,
        Spatial::Grid::Point::Value z,
        Arca::Index<Asset::Material> material)
        :
        ClosedTypedRelic(init), points(points), z(z), material(material)
    {}
}