
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

    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(Angle)
    {
        if (scribe.IsOutput())
        {
            auto& outputScribe = *scribe.AsOutput();

            outputScribe.Save(static_cast<ValueT>(underlying));
            outputScribe.Save(selectedType.SelectedAsID());
        }
        else // INPUT
        {
            auto& inputScribe = *scribe.AsInput();

            ValueT value;
            inputScribe.Load(value);
            underlying = value;

            SelectedType::ID selectedID;
            inputScribe.Load(selectedID);
            selectedType.Select(selectedID);
        }
    }
}