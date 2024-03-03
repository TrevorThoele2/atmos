#pragma once

#include "Serialization.h"

namespace Atmos
{
    class Acumen
    {
    public:
        typedef unsigned char ValueT;
        // When below one set, cannot use an ability
        constexpr static ValueT valuePerSet = 3;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT value;
    public:
        Acumen();
        Acumen(ValueT value);
        Acumen(const Acumen &arg) = default;
        Acumen& operator=(ValueT value);
        Acumen& operator=(const Acumen &arg) = default;
        bool operator==(const Acumen &arg) const;
        bool operator!=(const Acumen &arg) const;

        Acumen operator+(const Acumen &arg) const;
        Acumen operator+(ValueT value) const;
        Acumen operator-(const Acumen &arg) const;
        Acumen operator-(ValueT value) const;
        Acumen& operator+=(const Acumen &arg);
        Acumen& operator+=(ValueT value);
        Acumen& operator-=(const Acumen &arg);
        Acumen& operator-=(ValueT value);

        explicit operator ValueT() const;

        void Set(ValueT set);
        ValueT Get() const;
    };
}