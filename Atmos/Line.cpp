#include "Line.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Render
{
    Line::Line(Init init, const std::vector<Position2D>& points) :
        ClosedTypedRelic(init), points(points)
    {}

    Line::Line(
        Init init,
        const std::vector<Position2D>& points,
        Position2D::Value z,
        Arca::Index<Asset::Material> material,
        LineWidth width,
        Color color)
        :
        ClosedTypedRelic(init),
        points(points), z(z), material(material), width(width), color(color)
    {}
}

namespace Inscription
{
    void Scribe<Atmos::Render::Line, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.points);
        archive(object.z);
        archive(object.material);
        archive(object.width);
        archive(object.color);
    }
}