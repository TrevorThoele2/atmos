
#include "FixedPoint.h"

namespace Atmos
{
    RadixPoint& RadixPoint::operator=(Value arg)
    {
        value = arg;
        return *this;
    }

    bool RadixPoint::operator==(const RadixPoint& arg) const
    {
        return value == arg.value;
    }

    bool RadixPoint::operator!=(const RadixPoint& arg) const
    {
        return !(*this == arg);
    }

    RadixPoint::operator Value() const
    {
        return value;
    }

    RadixPoint::Value RadixPoint::Get() const
    {
        return value;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::RadixPoint, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.value);
    }

    void Scribe<::Atmos::RadixPoint, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}