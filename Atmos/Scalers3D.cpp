#include "Scalers3D.h"

namespace Atmos
{
    Scalers3D::Scalers3D() : x(1.0f), y(1.0f)
    {}

    Scalers3D::Scalers3D(Value x, Value y, Value z) : x(x), y(y), z(z)
    {}

    Scalers3D& Scalers3D::operator=(const Scalers3D& arg)
    {
        x = arg.x;
        y = arg.y;
        z = arg.z;
        return *this;
    }

    bool Scalers3D::operator==(const Scalers3D& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Scalers3D::operator!=(const Scalers3D& arg) const
    {
        return (*this == arg);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Scalers3D, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
        archive(object.z);
    }
}