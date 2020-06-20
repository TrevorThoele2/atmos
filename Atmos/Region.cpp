#include "Region.h"

namespace Atmos::Render
{
    Region::Region(Init init) :
        ClosedTypedRelic(init)
    {}

    Region::Region(
        Init init,
        const std::vector<Position2D>& points,
        Position2D::Value z,
        Arca::Index<Asset::Material> material)
        :
        ClosedTypedRelic(init), points(points), z(z), material(material)
    {}
}

namespace Inscription
{
    void Scribe<Atmos::Render::Region, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.points);
        archive(object.z);
        archive(object.material);
    }
}