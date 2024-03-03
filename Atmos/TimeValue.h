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
    public:
        TimeValue(ValueT value = ValueT(), EpochT epoch = EpochT::SECONDS);
        TimeValue(ValueT::ValueT value, EpochT epoch);
        TimeValue(const TimeValue& arg) = default;

        TimeValue& operator=(const TimeValue& arg) = default;

        bool operator==(const TimeValue& arg) const;
        bool operator!=(const TimeValue& arg) const;
        bool operator<(const TimeValue& arg) const;
        bool operator<=(const TimeValue& arg) const;
        bool operator>(const TimeValue& arg) const;
        bool operator>=(const TimeValue& arg) const;

        TimeValue operator+(const TimeValue& arg) const;
        TimeValue& operator+=(const TimeValue& arg);
        TimeValue operator-(const TimeValue& arg) const;
        TimeValue& operator-=(const TimeValue& arg);
        TimeValue operator*(const TimeValue& arg) const;
        TimeValue& operator*=(const TimeValue& arg);
        TimeValue operator/(const TimeValue& arg) const;
        TimeValue& operator/=(const TimeValue& arg);

        explicit operator double() const;

        explicit operator ValueT() const;
        ValueT Get() const;
        Radix GetRadixPoint() const;
        static Radix GetRadixPoint(EpochT epoch);

        void Convert(EpochT epoch);
        ValueT ConvertValue(EpochT epoch) const;
        EpochT GetEpoch() const;
    private:
        ValueT value;
        EpochT epoch;
    private:
        static ValueT ConvertValueStatic(ValueT value, EpochT oldEpoch, EpochT newEpoch, bool manipulateValue = false);
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}