#pragma once

#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos
{
    class Percentage
    {
    public:
        typedef FixedPoint64 WrappedT;
        typedef WrappedT::Split Split;

        static const WrappedT max;
        static const WrappedT min;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        WrappedT wrapped;
    public:
        Percentage() = default;
        Percentage(const WrappedT::Split &split);
        Percentage(const Percentage &arg) = default;
        Percentage& operator=(const WrappedT::Split &split);
        Percentage& operator=(const Percentage &arg) = default;

        bool operator==(const Percentage &arg) const;
        bool operator==(const WrappedT &arg) const;
        bool operator==(const Split &arg) const;
        bool operator!=(const Percentage &arg) const;
        bool operator!=(const WrappedT &arg) const;
        bool operator!=(const Split &arg) const;
        bool operator<(const Percentage &arg) const;
        bool operator<(const WrappedT &arg) const;
        bool operator<(const Split &arg) const;
        bool operator<=(const Percentage &arg) const;
        bool operator<=(const WrappedT &arg) const;
        bool operator<=(const Split &arg) const;
        bool operator>(const Percentage &arg) const;
        bool operator>(const WrappedT &arg) const;
        bool operator>(const Split &arg) const;
        bool operator>=(const Percentage &arg) const;
        bool operator>=(const WrappedT &arg) const;
        bool operator>=(const Split &arg) const;

        Percentage operator+(const Percentage &arg) const;
        Percentage operator+(const WrappedT &arg) const;
        Percentage operator+(const Split &arg) const;
        Percentage operator-(const Percentage &arg) const;
        Percentage operator-(const WrappedT &arg) const;
        Percentage operator-(const Split &arg) const;
        Percentage operator*(const Percentage &arg) const;
        Percentage operator*(const WrappedT &arg) const;
        Percentage operator*(const Split &arg) const;
        Percentage operator/(const Percentage &arg) const;
        Percentage operator/(const WrappedT &arg) const;
        Percentage operator/(const Split &arg) const;
        Percentage& operator+=(const Percentage &arg);
        Percentage& operator+=(const WrappedT &arg);
        Percentage& operator+=(const Split &arg);
        Percentage& operator-=(const Percentage &arg);
        Percentage& operator-=(const WrappedT &arg);
        Percentage& operator-=(const Split &arg);
        Percentage& operator*=(const Percentage &arg);
        Percentage& operator*=(const WrappedT &arg);
        Percentage& operator*=(const Split &arg);
        Percentage& operator/=(const Percentage &arg);
        Percentage& operator/=(const WrappedT &arg);
        Percentage& operator/=(const Split &arg);

        Percentage& operator++();
        Percentage& operator--();
        Percentage operator++(int);
        Percentage operator--(int);

        explicit operator char() const;
        explicit operator unsigned char() const;
        explicit operator short() const;
        explicit operator unsigned short() const;
        explicit operator int() const;
        explicit operator unsigned int() const;
        explicit operator long long() const;
        explicit operator unsigned long long() const;
        explicit operator float() const;
        explicit operator double() const;
        explicit operator Split() const;
        explicit operator WrappedT() const;

        void Floor();
        void Ceiling();

        // 100 = 1
        // 95 = 0.95
        Percentage ConvertToDecimal() const;
    };
}