#pragma once

#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos
{
    enum class TimeValueEpoch
    {
        MINUTES,
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS
    };

    class TimeValue
    {
    public:
        typedef TimeValueEpoch EpochT;
        typedef FixedPoint64 ValueT;
        typedef ValueT::Radix Radix;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT value;
        EpochT epoch;

        static ValueT ConvertValueStatic(ValueT value, EpochT newEpoch, bool manipulateValue = false);
    public:
        TimeValue(ValueT value = ValueT(), EpochT epoch = EpochT::SECONDS);
        TimeValue(ValueT::ValueT value, EpochT epoch);
        TimeValue(const TimeValue &arg) = default;
        TimeValue& operator=(const TimeValue &arg) = default;
        bool operator==(const TimeValue &arg) const;
        bool operator!=(const TimeValue &arg) const;
        bool operator>(const TimeValue &arg) const;
        bool operator>=(const TimeValue &arg) const;
        bool operator<(const TimeValue &arg) const;
        bool operator<=(const TimeValue &arg) const;
        TimeValue operator+(const TimeValue &other) const;
        TimeValue& operator+=(const TimeValue &other);
        TimeValue operator-(const TimeValue &other) const;
        TimeValue& operator-=(const TimeValue &other);
        TimeValue operator*(const TimeValue &other) const;
        TimeValue& operator*=(const TimeValue &other);
        TimeValue operator/(const TimeValue &other) const;
        TimeValue& operator/=(const TimeValue &other);

        // Needed for Timer's in Affecter to work
        explicit operator double() const;

        explicit operator ValueT() const;
        ValueT Get() const;
        Radix GetRadixPoint() const;
        static Radix GetRadixPoint(EpochT epoch);

        void Convert(EpochT epoch);
        ValueT ConvertValue(EpochT epoch) const;
        EpochT GetEpoch() const;
    };
}