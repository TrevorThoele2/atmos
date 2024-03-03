#include "Line.h"

namespace Atmos::Render
{
    Line::Line(Init init, Position2D from, Position2D to) :
        ClosedTypedRelic(init), from(from), to(to)
    {}
}

namespace Inscription
{
    void Scribe<Atmos::Render::Line, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.from);
        archive(object.to);
        archive(object.width);
        archive(object.color);
    }
}