#pragma once

#include "AngleConversion.h"
#include "AngleUnitsTypeValidation.h"

#include "Optional.h"

#include "Serialization.h"

#include <Chroma/SelectableType.h>

namespace Atmos
{
    class Angle
    {
    public:
        typedef float ValueT;
    private:
        template<class T>
        using AllowOnlyAngleUnits = typename std::enable_if<IsAngleUnits<T>::value, int>::type;
    public:
        Angle();
        template<class T, AllowOnlyAngleUnits<T> = 0>
        Angle(T arg);

        template<class T, AllowOnlyAngleUnits<T> = 0>
        Angle& operator=(T arg);

        Angle operator+(const Angle& arg) const;
        Angle& operator+=(const Angle& arg);
        Angle operator-(const Angle& arg) const;
        Angle& operator-=(const Angle& arg);
        Angle operator*(const Angle& arg) const;
        Angle& operator*=(const Angle& arg);
        Angle operator/(const Angle& arg) const;
        Angle& operator/=(const Angle& arg);

        bool operator==(const Angle& arg) const;
        bool operator!=(const Angle& arg) const;
        bool operator<(const Angle& arg) const;
        bool operator<=(const Angle& arg) const;
        bool operator>(const Angle& arg) const;
        bool operator>=(const Angle& arg) const;

        template<class T, AllowOnlyAngleUnits<T> = 0>
        T As() const;

        template<class T, AllowOnlyAngleUnits<T> = 0>
        bool Is() const;
    private:
        Radians underlying;
    private:
        typedef ::Chroma::SelectableType<Radians, Degrees> SelectedType;
        SelectedType selectedType;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class T, Angle::AllowOnlyAngleUnits<T>>
    Angle::Angle(T arg) : underlying(AngleConverter<T>::From(arg)), selectedType(::Chroma::Type<T>{})
    {}

    template<class T, Angle::AllowOnlyAngleUnits<T>>
    Angle& Angle::operator=(T arg)
    {
        underlying = AngleConverter<T>::From(arg);
        selectedType.Select<T>();
        return *this;
    }

    template<class T, Angle::AllowOnlyAngleUnits<T>>
    T Angle::As() const
    {
        return AngleConverter<T>::To(underlying);
    }

    template<class T, Angle::AllowOnlyAngleUnits<T>>
    bool Angle::Is() const
    {
        return selectedType.Is<T>();
    }
}