#include "GridRegion.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Render
{
    GridRegion::GridRegion(Init init) :
        ClosedTypedRelic(init)
    {}

    GridRegion::GridRegion(
        Init init,
        const std::vector<Grid::Position>& points,
        Grid::Position::Value z,
        Arca::Index<Asset::Material> material)
        :
        ClosedTypedRelic(init), points(points), z(z), material(material)
    {}
}

namespace Inscription
{
    void Scribe<Atmos::Render::GridRegion, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.points);
        archive(object.z);
        archive(object.material);
    }
}