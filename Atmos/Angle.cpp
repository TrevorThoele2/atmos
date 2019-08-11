
#include "Angle.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Angle::Angle() : selectedType(::Chroma::Type<Radians>{})
    {}

    Angle Angle::operator+(const Angle& arg) const
    {
        return Angle(underlying + arg.underlying);
    }

    Angle& Angle::operator+=(const Angle& arg)
    {
        underlying += arg.underlying;
        return *this;
    }

    Angle Angle::operator-(const Angle& arg) const
    {
        return Angle(underlying - arg.underlying);
    }

    Angle& Angle::operator-=(const Angle& arg)
    {
        underlying -= arg.underlying;
        return *this;
    }

    Angle Angle::operator*(const Angle& arg) const
    {
        return Angle(underlying * arg.underlying);
    }

    Angle& Angle::operator*=(const Angle& arg)
    {
        underlying *= arg.underlying;
        return *this;
    }

    Angle Angle::operator/(const Angle& arg) const
    {
        return Angle(underlying / arg.underlying);
    }

    Angle& Angle::operator/=(const Angle& arg)
    {
        underlying /= arg.underlying;
        return *this;
    }

    bool Angle::operator==(const Angle& arg) const
    {
        return underlying == arg.underlying && selectedType == arg.selectedType;
    }

    bool Angle::operator!=(const Angle& arg) const
    {
        return !(*this == arg);
    }

    bool Angle::operator<(const Angle& arg) const
    {
        return underlying < arg.underlying;
    }

    bool Angle::operator<=(const Angle& arg) const
    {
        return underlying <= arg.underlying;
    }

    bool Angle::operator>(const Angle& arg) const
    {
        return underlying > arg.underlying;
    }

    bool Angle::operator>=(const Angle& arg) const
    {
        return underlying >= arg.underlying;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Angle, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            auto underlying = static_cast<ObjectT::Value>(object.underlying);
            archive(underlying);

            auto selectedID = object.selectedType.SelectedAsID();
            archive(selectedID);
        }
        else // INPUT
        {
            ObjectT::Value value;
            archive(value);
            object.underlying = value;

            ObjectT::SelectedType::ID selectedID;
            archive(selectedID);
            object.selectedType.Select(selectedID);
        }
    }

    void Scribe<::Atmos::Angle, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}