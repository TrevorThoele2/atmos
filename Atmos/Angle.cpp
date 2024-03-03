
#include "Angle.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Angle::Angle() : selectedType(::Chroma::Type<Radians>{})
    {}

    Angle Angle::operator+(const Angle &arg) const
    {
        return Angle(underlying + arg.underlying);
    }

    Angle& Angle::operator+=(const Angle &arg)
    {
        underlying += arg.underlying;
        return *this;
    }

    Angle Angle::operator-(const Angle &arg) const
    {
        return Angle(underlying - arg.underlying);
    }

    Angle& Angle::operator-=(const Angle &arg)
    {
        underlying -= arg.underlying;
        return *this;
    }

    Angle Angle::operator*(const Angle &arg) const
    {
        return Angle(underlying * arg.underlying);
    }

    Angle& Angle::operator*=(const Angle &arg)
    {
        underlying *= arg.underlying;
        return *this;
    }

    Angle Angle::operator/(const Angle &arg) const
    {
        return Angle(underlying / arg.underlying);
    }

    Angle& Angle::operator/=(const Angle &arg)
    {
        underlying /= arg.underlying;
        return *this;
    }

    bool Angle::operator==(const Angle &arg) const
    {
        return underlying == arg.underlying && selectedType == arg.selectedType;
    }

    bool Angle::operator!=(const Angle &arg) const
    {
        return !(*this == arg);
    }

    bool Angle::operator<(const Angle &arg) const
    {
        return underlying < arg.underlying;
    }

    bool Angle::operator<=(const Angle &arg) const
    {
        return underlying <= arg.underlying;
    }

    bool Angle::operator>(const Angle &arg) const
    {
        return underlying > arg.underlying;
    }

    bool Angle::operator>=(const Angle &arg) const
    {
        return underlying >= arg.underlying;
    }

    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(Angle)
    {
        if (scribe.IsOutput())
        {
            scribe.Save(static_cast<ValueT>(underlying));
            scribe.Save(selectedType.SelectedAsID());
        }
        else // INPUT
        {
            ValueT value;
            scribe.Load(value);
            underlying = value;

            SelectedType::ID selectedID;
            scribe.Load(selectedID);
            selectedType.Select(selectedID);
        }
    }
}