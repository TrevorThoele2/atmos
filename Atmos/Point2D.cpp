#include "Point2D.h"

#include <Inscription/Scribe.h>

namespace Atmos::Spatial
{
    Point2D::Point2D(Value x, Value y) : x(x), y(y)
    {}

    bool Point2D::operator==(const Point2D& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Point2D::operator!=(const Point2D& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Spatial::Point2D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
    }
}