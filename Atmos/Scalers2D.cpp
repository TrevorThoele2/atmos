#include "Scalers2D.h"

namespace Atmos
{
    bool Scalers2D::operator==(const Scalers2D& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Scalers2D::operator!=(const Scalers2D& arg) const
    {
        return !(*this == arg);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Scalers2D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
    }
}