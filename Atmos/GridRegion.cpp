#include "GridRegion.h"

namespace Atmos::Render
{
    GridRegion::GridRegion(
        const std::unordered_set<Spatial::Grid::Point>& points,
        Spatial::Grid::Point::Value z,
        Arca::Index<Asset::Material> material)
        :
        points(points), z(z), material(material)
    {}
}